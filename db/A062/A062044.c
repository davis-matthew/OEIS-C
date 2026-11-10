/*
 * A062044  Primes arising in A062042.
 * https://oeis.org/A062044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(n) = A062042(n-1) + A062042(n) for n >= 2, where
 *            A062042(1) = 2 and A062042(k) = least x > A062042(k-1) with
 *            A062042(k-1) + x prime (computed by a direct search)
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static bool A062044_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    if (m % 3 == 0) return m == 3;
    for (uint64_t d = 5; d * d <= m; d += 6) {
        if (m % d == 0 || m % (d + 2) == 0) return false;
    }
    return true;
}

static uint64_t *A062044_b = NULL;   /* terms of A062042 */
static size_t A062044_cnt = 0;
static size_t A062044_cap = 0;

static void A062044_extend(size_t n)
{
    if (A062044_b == NULL) {
        A062044_cap = 64;
        A062044_b = (uint64_t *)malloc(A062044_cap * sizeof(uint64_t));
    }
    while (A062044_cnt < n) {
        uint64_t val;
        if (A062044_cnt == 0) {
            val = 2;
        } else {
            uint64_t prev = A062044_b[A062044_cnt - 1];
            uint64_t cand = prev + 1;
            while (!A062044_isprime(prev + cand)) cand++;
            val = cand;
        }
        if (A062044_cnt == A062044_cap) {
            A062044_cap <<= 1;
            A062044_b = (uint64_t *)realloc(A062044_b, A062044_cap * sizeof(uint64_t));
        }
        A062044_b[A062044_cnt++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;
    A062044_extend((size_t)n);
    uint64_t s = A062044_b[n - 1] + A062044_b[n - 2];
    return (seq_t)s;
}
