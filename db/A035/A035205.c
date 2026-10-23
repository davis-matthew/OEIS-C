/*
 * A035205  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          23.
 * https://oeis.org/A035205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(23,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This uses the multiplicative formula derived from the OEIS description) */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1, but guard against zero */

    idx_t m = n;
    seq_t res = 1;

    /* factor out the prime 2 */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        /* (23|2) = 1, so a(2^e) = e+1 */
        res *= (seq_t)(e + 1);
        if (res == 0)
            return 0;
    }

    /* trial division by odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t comp;
            if (p == 23) {
                comp = 1;               /* a(23^e) = 1 */
            } else {
                int symbol = jacobi_symbol(23, p);
                if (symbol == -1) {
                    comp = ((e & 1) == 0) ? 1 : 0;   /* (1+(-1)^e)/2 */
                } else {                              /* symbol == 1 */
                    comp = (seq_t)(e + 1);
                }
            }
            res *= comp;
            if (res == 0)
                return 0;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        idx_t p = m;
        idx_t e = 1;   /* exponent is 1 */

        seq_t comp;
        if (p == 23) {
            comp = 1;
        } else {
            int symbol = jacobi_symbol(23, p);
            if (symbol == -1) {
                comp = 0; /* e is odd, so (1+(-1)^1)/2 = 0 */
            } else {
                comp = (seq_t)(e + 1); /* e+1 = 2 */
            }
        }
        res *= comp;
    }

    return res;
}
