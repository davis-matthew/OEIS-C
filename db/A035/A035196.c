/*
 * A035196  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          14.
 * https://oeis.org/A035196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(14,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A035196.
 * Uses the multiplicative description:
 *   a(p^e) = 1                 if p = 2 or 7,
 *   a(p^e) = (e + 1)           if Kronecker(14,p) =  1,
 *   a(p^e) = (1 + (-1)^e)/2    if Kronecker(14,p) = -1.
 *
 * The Kronecker symbol with numerator 14 factors as (2,p)*(7,p).
 *  (2,p) is 1 for p ≡ ±1 mod 8 and -1 for p ≡ ±3 mod 8.
 *  (7,p) is the Legendre symbol (7|p) which is obtained via jacobi_symbol.
 *
 * The function factors n by calling factor_u64, then multiplies the
 * contributions of each prime power, exploiting the multiplicativity of
 * a(n).  Results fit in seq_t for any n that fits in idx_t.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"

/* Helper: compute Kronecker(14,p) for a prime p. */
static int kronecker14(uint64_t p) {
    if (p == 2ULL || p == 7ULL) return 0;          /* gcd(14,p) > 1 → 0 */
    int k2;
    switch (p & 7ULL) {                            /* p mod 8 */
        case 1: case 7: k2 =  1; break;           /* 1 mod 8 or 7 mod 8 */
        case 3: case 5: k2 = -1; break;           /* 3 mod 8 or 5 mod 8 */
        default: k2 =  0; break;                  /* should not happen for odd p */
    }
    int k7 = jacobi_symbol(7ULL, p);               /* (7|p) */
    return k2 * k7;                                /* product gives (14|p) */
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return 1;                          /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;                     /* idx_t is uint64_t */

    /* Factor n.  64 slots is far more than enough for uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 2ULL || p == 7ULL) {
            val = 1;                                /* rule for p = 2 or 7 */
        } else {
            int kr = kronecker14(p);
            if (kr == 1) {
                val = (seq_t)(e + 1);               /* e+1 */
            } else {                                /* kr == -1 */
                val = ((e & 1ULL) == 0ULL) ? 1 : 0; /* 1 if e even, else 0 */
            }
        }

        if (val == 0) return 0;                    /* any zero factor kills the product */
        result *= val;
    }

    return result;
}
