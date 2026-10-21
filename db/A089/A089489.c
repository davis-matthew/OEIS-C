/*
 * A089489  Smallest k greater than n such that (k^2-n^2, 2*k*n, k^2+n^2) is a
 *          primitive Pythagorean triple and the corresponding right triangle
 *          has a prime hypotenuse.
 * https://oeis.org/A089489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n such that k^2 + n^2 is prime, gcd(k,n)=1
 *            and k,n have opposite parity (so (k^2-n^2,2kn,k^2+n^2) is a
 *            primitive Pythagorean triple with prime hypotenuse)
 * Range:     u8 n <= 11, u16 n <= 470, u32 n <= 470, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* n is >= 1 */
    seq_t k = (seq_t)n + 1;                /* start searching from n+1   */
    seq_t n_sq = (seq_t)n * (seq_t)n;       /* n^2, computed once       */
    for (;;) {
        seq_t sum = k * k + n_sq;          /* k^2 + n^2               */
        if (is_prime_big(sum)) {
            if (gcd(k, (seq_t)n) == 1 &&
                ((k ^ (seq_t)n) & 1) != 0)  /* opposite parity         */
            {
                return k;
            }
        }
        ++k;
    }
}
