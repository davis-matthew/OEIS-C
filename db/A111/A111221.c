/*
 * A111221  d_11(n), tau_11(n), number of ordered factorizations of n as n =
 *          rstuvwxyzab (11-factorizations).
 * https://oeis.org/A111221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏_{p^e∥n} binomial(e+10,10) (OEIS A111221)
 * Range:     u8 n <= 7, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1U) return (seq_t)1;

    idx_t m = n;
    seq_t res = (seq_t)1;
    idx_t limit = (idx_t)int_sqrt(m);

    for (idx_t p = 2; p <= limit; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            res *= binomial(e + 10, (idx_t)10);
            limit = (idx_t)int_sqrt(m);
        }
    }

    if (m > 1)        /* remaining prime factor with exponent 1 */
        res *= binomial((idx_t)11, (idx_t)10);   /* binomial(1+10,10) = 11 */

    return res;
}
