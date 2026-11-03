/*
 * A220431  Number of ways to write n=x+y (x>0, y>0) with 3x-1, 3x+1 and xy-1
 *          all prime.
 * https://oeis.org/A220431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 2216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9370 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Count the number of ordered pairs (x,y) such that
   x>0, y>0, x+y = n, and 3x-1, 3x+1, and xy-1 are all prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;
        uint64_t p1 = 3 * (uint64_t)x - 1;           /* 3x - 1   */
        uint64_t p2 = 3 * (uint64_t)x + 1;           /* 3x + 1   */
        uint64_t p3 = (uint64_t)x * (uint64_t)y - 1; /* xy - 1    */

        if (is_prime_seq(p1) && is_prime_seq(p2) && is_prime_seq(p3)) {
            ++count;
        }
    }
    return count;
}
