/*
 * A034718  Dirichlet convolution of b_n=n with b_n with b_n.
 * https://oeis.org/A034718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * product_{p^e||n} binomial(e+2, 2) where
 *            binomial(e+2,2) = (e+2)*(e+1)/2. The function computes the
 *            Dirichlet convolution of n with itself twice by factoring n and
 *            multiplying the appropriate binomial factors
 * Range:     u8 n <= 17, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;          /* start with the factor n itself */
    uint64_t temp   = n;              /* use uint64_t for trial division */
    uint64_t p;

    for (p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* binomial(e+2, 2) = (e+2)*(e+1)/2 */
            seq_t factor = (seq_t)(e + 2) * (e + 1) / 2;
            result *= factor;
        }
    }
    if (temp > 1) {            /* remaining prime factor of exponent 1 */
        seq_t factor = (seq_t)(1 + 2) * (1 + 1) / 2;  /* =3 */
        result *= factor;
    }

    return result;
}
