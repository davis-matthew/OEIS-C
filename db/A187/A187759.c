/*
 * A187759  Number of ways to write n=x+y (0<x<y<n) with 6x-1, 6x+1, 6y-1 and
 *          6y+1 all prime.
 * https://oeis.org/A187759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with 0 < x < y < n, x + y =
 *            n, and all of 6x-1, 6x+1, 6y-1, 6y+1 are prime
 * Range:     u8 n <= 9469, u16 n <= 9469, u32 n <= 9469, u64 n <= 9469,
 *            u128 n <= 9469, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x * 2 < n; ++x) {        /* loop over 0 < x < n/2 */
        idx_t y = n - x;
        if (is_prime(6 * x - 1) &&
            is_prime(6 * x + 1) &&
            is_prime(6 * y - 1) &&
            is_prime(6 * y + 1)) {
            ++count;
        }
    }
    return count;
}
