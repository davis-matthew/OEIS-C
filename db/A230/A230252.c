/*
 * A230252  Number of ways to write n = x + y (x, y > 0) with 2*x + 1, x^2 + x
 *          + 1 and y^2 + y + 1 all prime.
 * https://oeis.org/A230252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y) : x>0, y>0, x+y=n, 2*x+1 prime, x^2+x+1 prime,
 *            y^2+y+1 prime}| (Ordered pairs; so both (x,y) and (y,x) are
 *            counted if valid)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;

        idx_t p1 = 2 * x + 1;
        idx_t p2 = x * x + x + 1;
        idx_t p3 = y * y + y + 1;

        if (is_prime(p1) && is_prime(p2) && is_prime(p3)) {
            ++count;
        }
    }
    return count;
}
