/*
 * A078929  Least k > 0 such that A000002(n+k) = A000002(n).
 * https://oeis.org/A078929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that A000002(n+k) = A000002(n), where
 *            A000002 is the Kolakoski sequence (computed by search, k <= 3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A000002_kol_buf = NULL;
static uint64_t A000002_kol_cap = 0;
static uint64_t A000002_kol_len = 0;
static uint64_t A000002_kol_run = 0;

static int A000002_kol_ensure(uint64_t need)
{
    if (need + 3 > A000002_kol_cap) {
        uint64_t nc = A000002_kol_cap ? A000002_kol_cap : 64;
        unsigned char *nb;
        while (nc < need + 3) nc *= 2;
        nb = (unsigned char *)realloc(A000002_kol_buf, (size_t)nc);
        if (!nb) return 0;
        A000002_kol_buf = nb;
        A000002_kol_cap = nc;
    }
    return 1;
}

static seq_t A000002_a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!A000002_kol_ensure((uint64_t)n)) return (seq_t)0;
    if (A000002_kol_len == 0) {
        A000002_kol_buf[1] = 1;
        A000002_kol_buf[2] = 2;
        A000002_kol_buf[3] = 2;
        A000002_kol_len = 3;
        A000002_kol_run = 3;
    }
    while (A000002_kol_len < (uint64_t)n) {
        unsigned char v = (unsigned char)((A000002_kol_run & 1) ? 1 : 2);
        unsigned char L = A000002_kol_buf[A000002_kol_run];
        unsigned char k;
        if (!A000002_kol_ensure(A000002_kol_len + 2)) return (seq_t)0;
        for (k = 0; k < L; k++)
            A000002_kol_buf[++A000002_kol_len] = v;
        A000002_kol_run++;
    }
    return (seq_t)A000002_kol_buf[n];
}

seq_t a(idx_t n)
{
    seq_t base = A000002_a(n);
    idx_t k = 1;
    while (A000002_a(n + k) != base) {
        k++;
        if (k > 8) break;
    }
    return (seq_t)k;
}
