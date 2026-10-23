/*
 * A155177  Area ar/6 (divided by 6) of primitive Pythagorean triangles such
 *          that perimeters are Averages of twin prime pairs, q=p+1,
 *          a=q^2-p^2, c=q^2+p^2, b=2*p*q, ar=a*b/2; s=a+b+c, s-+1 are primes.
 * https://oeis.org/A155177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2p+1)*(p^2+p)/6 where p is the smallest integer such
 *            that 4p^2+6p+1 and 4p^2+6p+3 are both prime. The sequence tables
 *            the resulting values in order of increasing p
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t p = 1;
    for (;; ++p) {
        seq_t p_t = (seq_t)p;

        /* numbers to test for primality: 4p^2+6p+1 and +2 */
        seq_t x = 4 * p_t * p_t + 6 * p_t + 1;
        seq_t y = x + 2;

        if (is_prime((idx_t)x) && is_prime((idx_t)y)) {
            ++count;
            if (count == n) {
                /* a(n) = (2p+1)*(p^2+p)/6 */
                seq_t res = (2 * p_t + 1) * (p_t * p_t + p_t) / 6;
                return res;
            }
        }
    }
}
