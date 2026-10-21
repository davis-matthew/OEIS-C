/*
 * A218585  Number of ways to write n as x+y with 0<x<=y and x^2+xy+y^2 prime.
 * https://oeis.org/A218585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (x,y) | 0 < x <= y, x + y = n, x^2 + x*y + y^2 is
 *            prime }
 * Range:     u8 n <= 16, u16 n <= 421, u32 n <= 421, u64 n <= 9238,
 *            u128 n <= 9238, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Count admissible (x,y) pairs. */
    idx_t half = n >> 1;          /* floor(n / 2) */
    idx_t i;
    seq_t count = 0;

    for (i = 1; i <= half; ++i)
    {
        idx_t y = n - i;
        seq_t x   = (seq_t)i;
        seq_t yy  = (seq_t)y;
        seq_t val = x * x + x * yy + yy * yy;
        if (is_prime_big(val))
            ++count;
    }
    return count;
}
