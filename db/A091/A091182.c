/*
 * A091182  Number of ways to write n = x + y (x >= y > 0) with xy - 1 and xy
 *          + 1 both prime.
 * https://oeis.org/A091182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 846, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 103 terms through n = 9733 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of unordered pairs (x,y) with x>=y>0, x+y=n,
   such that xy-1 and xy+1 are both prime. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* works for any call order */

    seq_t count = 0;
    idx_t half = n / 2;     /* y <= x -> y <= n/2 */

    for (idx_t y = 1; y <= half; ++y) {
        idx_t x = n - y;    /* x >= y automatically */
        seq_t prod = (seq_t)x * (seq_t)y;

        /* test both neighbours for primality */
        if (is_prime_seq(prod - 1) && is_prime_seq(prod + 1))
            ++count;
    }

    return count;
}
