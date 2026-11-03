/*
 * A035167  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -23.
 * https://oeis.org/A035167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-23,d) Computed by multiplicativity:
 *            factor n and multiply per prime power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9641 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for undefined index */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 23)
        {
            /* Kronecker(-23,23)=0, so contribution is 1 for any exponent */
            continue;
        }
        if (p == 2)
        {
            /* (D/2)=1 for D=-23 because D≡1 mod 8 */
            result *= (seq_t)(e + 1);
            continue;
        }
        /* Kronecker(-23,p) for odd p≠23: (-1)^((p-1)/2) * (23/p) */
        int sign_minus1 = (((p - 1) >> 1) & 1) ? -1 : 1;
        int leg = jacobi_symbol(23, p);      /* Legendre symbol (23/p) */
        int chi = sign_minus1 * leg;         /* Kronecker(-23,p) */

        if (chi == 1)
        {
            result *= (seq_t)(e + 1);
        }
        else if (chi == -1)
        {
            if (e & 1) return 0;            /* odd exponent yields zero */
            /* even exponent -> factor 1, no change */
        }
        /* chi == 0 cannot occur here because p==23 was handled earlier */
    }

    return result;
}
