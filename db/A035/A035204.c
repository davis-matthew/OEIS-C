/*
 * A035204  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          22.
 * https://oeis.org/A035204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(22,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = ∏_{p^e || n} a(p^e) where
 *              a(p^e) = 1          if Kronecker(22,p) = 0  (p=2 or 11)
 *              a(p^e) = (1+(-1)^e)/2 if Kronecker(22,p) = -1 (p quadratic non‑residue mod 22)
 *              a(p^e) = e+1       if Kronecker(22,p) = 1  (p quadratic residue mod 22) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* treat n==0 as 0, though not used by OEIS offset */
    if (n == 0) return 0;

    uint64_t N = (uint64_t)n;

    /* factorization buffer */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64(N, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];

        int k = jacobi_symbol(22, p); /* 0 if p|22, otherwise ±1 */

        seq_t val;
        if (k == 0) {
            val = 1;                       /* Kronecker = 0  → a(p^e)=1 */
        } else if (k == -1) {
            val = (exp & 1ULL) == 0 ? 1 : 0;/* (1+(-1)^e)/2 */
        } else { /* k == 1 */
            val = (seq_t)(exp + 1);         /* e+1 */
        }

        result *= val;
        if (result == 0) break;           /* early exit if product becomes zero */
    }

    return result;
}
