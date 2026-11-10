/*
 * A064957  Where the odd terms appear in A064413.
 * https://oeis.org/A064957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k of the n-th odd term of the EKG sequence A064413
 *            (computed by generating the EKG sequence with a per-prime
 *            pointer sieve and recording the indices of odd terms)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9907 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A064957_LIMIT 2000000

static uint32_t *A064957_spf = NULL;
static uint32_t *A064957_ptr = NULL;
static unsigned char *A064957_used = NULL;
static uint32_t *A064957_odd = NULL;
static size_t A064957_cnt = 0, A064957_cap = 0;
static uint32_t A064957_prev = 2;
static uint64_t A064957_idx = 2;

static void A064957_init(void)
{
    uint32_t i, j;
    A064957_spf = (uint32_t *)calloc(A064957_LIMIT + 1, sizeof(uint32_t));
    A064957_ptr = (uint32_t *)calloc(A064957_LIMIT + 1, sizeof(uint32_t));
    A064957_used = (unsigned char *)calloc(A064957_LIMIT + 1, 1);
    if (!A064957_spf || !A064957_ptr || !A064957_used) exit(1);
    for (i = 2; i <= A064957_LIMIT; ++i) {
        if (A064957_spf[i] == 0) {
            for (j = i; j <= A064957_LIMIT; j += i)
                if (A064957_spf[j] == 0) A064957_spf[j] = i;
            A064957_ptr[i] = i;
        }
    }
    A064957_cap = 1024;
    A064957_odd = (uint32_t *)malloc(A064957_cap * sizeof(uint32_t));
    if (!A064957_odd) exit(1);
    A064957_odd[A064957_cnt++] = 1; /* index 1 holds term 1 (odd) */
    A064957_used[1] = 1;
    A064957_used[2] = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A064957_spf) A064957_init();
    while (A064957_cnt < (size_t)n) {
        uint32_t x = A064957_prev, best = 0;
        while (x > 1) {
            uint32_t p = A064957_spf[x];
            uint32_t c = A064957_ptr[p];
            while (c <= A064957_LIMIT && A064957_used[c]) c += p;
            A064957_ptr[p] = c;
            if (best == 0 || c < best) best = c;
            while (x % p == 0) x /= p;
        }
        if (best == 0 || best > A064957_LIMIT) break;
        A064957_used[best] = 1;
        A064957_prev = best;
        A064957_idx++;
        if (best & 1) {
            if (A064957_cnt == A064957_cap) {
                A064957_cap *= 2;
                A064957_odd = (uint32_t *)realloc(A064957_odd, A064957_cap * sizeof(uint32_t));
                if (!A064957_odd) exit(1);
            }
            A064957_odd[A064957_cnt++] = (uint32_t)A064957_idx;
        }
    }
    if ((size_t)n > A064957_cnt) return 0;
    return (seq_t)A064957_odd[n - 1];
}
