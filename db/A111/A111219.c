/*
 * A111219  d_9(n), tau_9(n), number of ordered factorizations of n as n =
 *          rstuvwxyz (9-factorizations).
 * https://oeis.org/A111219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∏_{p^e‖n} C(e+8,8)   (multiplicative, a(p^e)=C(e+8,8))
 */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t  res = (seq_t)1;
    idx_t  m   = n;

    /* trial division */
    for (idx_t d = 2; d <= int_sqrt(m); ++d) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            res *= binomial(e + 8, 8);
        }
    }

    /* prime factor > sqrt(n) */
    if (m > 1) {
        /* remaining exponent is 1 */
        res *= binomial(9, 8);   /* C(9,8)=9 */
    }

    return res;
}
