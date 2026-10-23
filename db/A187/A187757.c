/*
 * A187757  Number of ways to write n=x+y (x,y>0) with 6x-1, 6x+1, 6y+1 and
 *          6y+5 all prime.
 * https://oeis.org/A187757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of ways to write n = x + y (x,y>0) such that 6x-1,
 *            6x+1, 6y+1 and 6y+5 are all prime
 * Range:     u8 n <= 9467, u16 n <= 9467, u32 n <= 9467, u64 n <= 9467,
 *            u128 n <= 9467, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility header providing is_prime() */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;
        if (is_prime((6 * x) - 1) &&
            is_prime((6 * x) + 1) &&
            is_prime((6 * y) + 1) &&
            is_prime((6 * y) + 5)) {
            ++count;
        }
    }
    return count;
}
