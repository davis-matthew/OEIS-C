/*
 * A086849  Sum of first n nonsquares.
 * https://oeis.org/A086849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (i + floor(1/2 + sqrt(i)))
 * Range:     u8 n <= 19, u16 n <= 349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9794 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *prefix = NULL;   /* cumulative sums, prefix[0] = 0 */
static idx_t prefix_max = 0;   /* maximum n computed so far */

/* Compute the n-th term of A086849 */
seq_t a(idx_t n)
{
    /* Ensure we have computed up to n */
    if (n > prefix_max) {
        /* Allocate or grow the array */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_prefix = mem_resize(seq_t, prefix, new_size);
        if (!new_prefix) {
            /* Out of memory; return 0 as a safe fallback */
            return (seq_t)0;
        }
        prefix = new_prefix;

        /* Initialize new entries */
        for (idx_t i = prefix_max + 1; i <= n; ++i) {
            seq_t idx = (seq_t)i;
            seq_t sq = floor_sqrt(idx);          /* floor(sqrt(i)) */
            seq_t round_sqrt;
            /* Determine round(sqrt(i)) using integer arithmetic */
            if (idx <= sq * sq + sq)
                round_sqrt = sq;
            else
                round_sqrt = sq + 1;
            seq_t val = idx + round_sqrt;        /* A000037(i) */
            prefix[i] = prefix[i - 1] + val;     /* cumulative sum */
        }
        prefix_max = n;
    }
    return prefix[n];
}
