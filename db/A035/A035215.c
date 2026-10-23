/*
 * A035215  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          33.
 * https://oeis.org/A035215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(33, d) (OEIS A035215)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute the Jacobi symbol (a/n) for odd n.  Returns 0, 1, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0)
        return 0;
    a %= n;
    int result = 1;
    while (a != 0) {
        /* Factor powers of 2 out of a */
        while ((a & 1) == 0) {
            a >>= 1;
            unsigned int n_mod8 = (unsigned int)(n & 7);
            if (n_mod8 == 3 || n_mod8 == 5)
                result = -result;
        }
        /* Swap a and n */
        uint64_t temp = a; a = n; n = temp;
        /* Quadratic reciprocity sign */
        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* Kronecker symbol (33/p) for an odd prime p */
static int kronecker_33_prime(uint64_t p) {
    /* p divides 33 => symbol 0 */
    if (p == 3 || p == 11)
        return 0;
    /* otherwise use Jacobi symbol */
    return jacobi_symbol(33, p);
}

/* Main sequence routine */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* offset is 1, n=0 is not used */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        int Kr = kronecker_33_prime(p);

        seq_t contrib;
        if (Kr == 0) {
            contrib = 1;                                 /* p = 3 or 11 */
        } else if (Kr == -1) {
            contrib = ((e & 1) == 0) ? 1 : 0;            /* (1+(-1)^e)/2 */
        } else { /* Kr == 1 */
            contrib = (seq_t)(e + 1);                   /* e + 1 */
        }

        result *= contrib;
        if (result == 0)                           /* early exit */
            break;
    }
    return result;
}
