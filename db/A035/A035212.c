/*
 * A035212  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          30.
 * https://oeis.org/A035212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(30,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative implementation using prime factorisation */

#include <stdint.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used by OEIS, but safe */
    uint64_t pr[64];                      /* prime factors */
    uint64_t ex[64];                      /* exponents   */
    size_t  cnt = 0;                      /* number of primes */
    factor_u64(n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* primes 2,3,5 divide 30 → contribution is always 1 */
        if (p == 2ULL || p == 3ULL || p == 5ULL)
            continue;

        /* Kronecker symbol (30/p) is ±1 because p ∤ 30  */
        int chi = jacobi_symbol(30ULL, p);

        if (chi == 1) {
            /* a(p^e) = e + 1 */
            res *= (seq_t)(e + 1);
        } else if (chi == -1) {
            /* a(p^e) = 1 if e even, else 0   */
            if (e & 1ULL) {            /* odd exponent → zero */
                return 0;              /* result becomes zero for all higher primes */
            }
            /* even exponent → multiply by 1, no change */
        } else {
            /* this should never happen for p coprime to 30 */
            return 0;
        }
    }
    return res;
}
