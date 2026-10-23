/*
 * A175396  Numbers whose sum of squares of digits is a square.
 * https://oeis.org/A175396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth number whose sum of squares of its decimal digits is
 *            a perfect square
 * Range:     u8 n <= 8, u16 n <= 2523, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_square_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_square_sum.h"
#include "../utils/floor_sqrt.h"

/* Test whether a 64‑bit sum is a perfect square */
static bool is_square_uint64(uint64_t sum)
{
    seq_t r = floor_sqrt((seq_t)sum);
    return (r * r) == (seq_t)sum;
}

/* Computes the n‑th term of A175396 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0 – return 0 for safety. */
        return 0;
    }

    idx_t candidate = 0;   /* start from 0, a(1) = 0 */
    idx_t found    = 0;    /* number of qualifying terms found so far */

    for (;;) {
        uint64_t ss = digit_square_sum(candidate);
        if (is_square_uint64(ss)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }

        /* Prevent wrap‑around of the candidate counter (should not happen
           for the required range). */
        if (candidate == UINT64_MAX) {
            break;
        }
        ++candidate;
    }

    /* In case n was larger than can be represented (unlikely) */
    return 0;
}
