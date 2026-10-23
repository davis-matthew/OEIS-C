/*
 * A035217  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          35.
 * https://oeis.org/A035217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(35, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined offset, safe guard */
    uint64_t nn = (uint64_t)n;

    /* n = 1 has no prime factors, but the product is defined to be 1 */
    if (nn == 1) return 1;

    /* factorization tables – 64 entries are more than enough for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib;

        if (p == 5 || p == 7) {          /* Kronecker(35, p) = 0      */
            contrib = 1;
        } else if (p == 2) {             /* Kronecker(35, 2) = -1     */
            contrib = (e % 2 == 0) ? 1 : 0;
        } else {
            int k = jacobi_symbol(35, p);   /* Legendre symbol (35|p)   */
            if (k < 0) {                   /* value –1:  (1+(-1)^e)/2  */
                contrib = (e % 2 == 0) ? 1 : 0;
            } else if (k > 0) {            /* value +1:  e+1            */
                contrib = (seq_t)(e + 1);
            } else {                       /* should not happen        */
                contrib = 1;
            }
        }
        result *= contrib;
    }
    return result;
}
