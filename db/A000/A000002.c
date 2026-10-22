/*
 * A000002  Kolakoski sequence: a(n) is length of n-th run; a(1) = 1; sequence
 *          consists just of 1's and 2's.
 * https://oeis.org/A000002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   Kolakoski self-generation: a(1)=1, a(2)=2, a(3)=2; for k >= 3,
 *            the k-th run has length a(k) and consists of 1's if k is odd,
 *            2's if k is even
 * Range:     u8 n <= 9898, u16 n <= 9898, u32 n <= 9898, u64 n <= 9898,
 *            u128 n <= 9898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *kol_buf = NULL;
static uint64_t kol_cap = 0;   /* allocated size */
static uint64_t kol_len = 0;   /* terms 1..kol_len are known */
static uint64_t kol_run = 0;   /* next run index to expand */

static int kol_ensure(uint64_t need)
{
    if (need + 3 > kol_cap) {
        uint64_t nc = kol_cap ? kol_cap : 64;
        unsigned char *nb;
        while (nc < need + 3) nc *= 2;
        nb = (unsigned char *)realloc(kol_buf, (size_t)nc);
        if (!nb) return 0;
        kol_buf = nb;
        kol_cap = nc;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!kol_ensure((uint64_t)n)) return (seq_t)0;
    if (kol_len == 0) {
        kol_buf[1] = 1;
        kol_buf[2] = 2;
        kol_buf[3] = 2;
        kol_len = 3;
        kol_run = 3;
    }
    while (kol_len < (uint64_t)n) {
        unsigned char v = (unsigned char)((kol_run & 1) ? 1 : 2);
        unsigned char L = kol_buf[kol_run];
        unsigned char k;
        if (!kol_ensure(kol_len + 2)) return (seq_t)0;
        for (k = 0; k < L; k++)
            kol_buf[++kol_len] = v;
        kol_run++;
    }
    return (seq_t)kol_buf[n];
}
