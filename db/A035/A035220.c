/*
 * A035220  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          38.
 * https://oeis.org/A035220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(38,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined; return 0 for safety */

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        int kr = jacobi_symbol(38ULL, p);   /* Kronecker(38,p) for prime p */

        seq_t factor;
        if (kr == 0) {                 /* p = 2 or 19, symbol 0 */
            factor = 1;
        } else if (kr == 1) {           /* symbol +1  */
            factor = (seq_t)(e + 1);
        } else {                        /* symbol -1  */
            factor = (e & 1ULL) ? (seq_t)0 : (seq_t)1;
        }

        result *= factor;
        if (result == (seq_t)0) break; /* early exit */
    }
    return result;
}
