/*
 * A064473  Where the odd terms appear in A064413 (if it begins at 2).
 * https://oeis.org/A064473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- generate the EKG sequence A064413 (a(1)=1, a(2)=2,
 *            then smallest unused number sharing a factor with the previous
 *            term); a(n) = (index of the n-th odd EKG term after the first) -
 *            1
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9508 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define A064473_LIM (1u << 22)

static uint32_t *A064473_spf = NULL;
static uint32_t *A064473_ptr = NULL;
static unsigned char *A064473_used = NULL;
static uint32_t *A064473_res = NULL;
static size_t A064473_cnt = 0, A064473_cap = 0;
static uint32_t A064473_last = 2, A064473_idx = 2;

static void A064473_init(void)
{
    if (A064473_spf) return;
    A064473_spf = (uint32_t *)calloc(A064473_LIM + 1, sizeof(uint32_t));
    A064473_ptr = (uint32_t *)calloc(A064473_LIM + 1, sizeof(uint32_t));
    A064473_used = (unsigned char *)calloc(A064473_LIM + 1, 1);
    for (uint32_t i = 2; i <= A064473_LIM; i++) {
        if (A064473_spf[i] == 0) {
            for (uint32_t j = i; j <= A064473_LIM; j += i)
                if (A064473_spf[j] == 0) A064473_spf[j] = i;
            if (i > 65536) continue;
        }
    }
    A064473_used[1] = 1;
    A064473_used[2] = 1;
}

static void A064473_step(void)
{
    uint32_t x = A064473_last, best = 0xFFFFFFFFu;
    while (x > 1) {
        uint32_t p = A064473_spf[x];
        while (x % p == 0) x /= p;
        uint32_t c = A064473_ptr[p];
        if (c == 0) c = p;
        while (c <= A064473_LIM && A064473_used[c]) c += p;
        A064473_ptr[p] = c;
        if (c < best) best = c;
    }
    if (best > A064473_LIM) exit(1);
    A064473_used[best] = 1;
    A064473_last = best;
    A064473_idx++;
    if (best & 1u) {
        if (A064473_cnt == A064473_cap) {
            A064473_cap = A064473_cap ? A064473_cap * 2 : 1024;
            A064473_res = (uint32_t *)realloc(A064473_res, A064473_cap * sizeof(uint32_t));
        }
        A064473_res[A064473_cnt++] = A064473_idx - 1;
    }
}

seq_t a(idx_t n)
{
    A064473_init();
    while ((idx_t)A064473_cnt < n) A064473_step();
    return (seq_t)A064473_res[n - 1];
}
