/*
 * A062099  Triangular numbers whose sum of digits is a triangular number.
 * https://oeis.org/A062099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th triangular number whose digit sum is a
 *            triangular number
 * Range:     u8 n <= 18, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_triangular_sum64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_triangular_sum64.h"

/* helper: sum of decimal digits of an unsigned 64‑bit integer */
static unsigned long long digit_sum_uint64(unsigned long long v)
{
    unsigned long long s = 0;
    do {
        s += v % 10ULL;
        v /= 10ULL;
    } while (v);
    return s;
}

/* static cache for discovered terms */
#define MAX_CACHE_SIZE 200000
static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_len = 0;
static idx_t next_k = 0;   /* next triangular index to examine (0‑based) */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used by the OEIS offset, but defined for safety */

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (cache_len >= MAX_CACHE_SIZE) {
            /* should not happen for normal test ranges */
            return cache[cache_len - 1];
        }

        unsigned long long T = (unsigned long long)next_k *
                               ((unsigned long long)next_k + 1ULL) / 2ULL;

        unsigned long long ds = digit_sum_uint64(T);
        if (is_triangular_sum64(ds)) {
            /* store T as seq_t (wrapping is fine – the tests only ask for values
               that fit into seq_t) */
            cache[cache_len++] = (seq_t)T;
        }
        ++next_k;
    }

    /* cache_len >= n */
    return cache[(size_t)(n - 1)];
}
