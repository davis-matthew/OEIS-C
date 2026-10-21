/*
 * A091182  Number of ways to write n = x + y (x >= y > 0) with xy - 1 and xy
 *          + 1 both prime.
 * https://oeis.org/A091182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 846, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of unordered pairs (x,y) with x>=y>0, x+y=n,
   such that xy-1 and xy+1 are both prime. */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* works for any call order */

    seq_t count = 0;
    idx_t half = n / 2;     /* y <= x -> y <= n/2 */

    for (idx_t y = 1; y <= half; ++y) {
        idx_t x = n - y;    /* x >= y automatically */
        seq_t prod = (seq_t)x * (seq_t)y;

        /* test both neighbours for primality */
        if (is_prime_small(prod - 1) && is_prime_small(prod + 1))
            ++count;
    }

    return count;
}
