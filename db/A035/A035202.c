/*
 * A035202  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          20.
 * https://oeis.org/A035202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(20,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

/* Return Kronecker(20,d); 0 for even d */
static inline int kronecker20(uint64_t d)
{
    return (d & 1) ? (int)jacobi_symbol(20, d) : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not part of the defined offset */

    uint64_t x = (uint64_t)n;
    seq_t res = 1;

    for (uint64_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            int e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }

            int k = kronecker20(p);
            seq_t ap;
            if (k == 0)                /* primes 2 and 5 */
                ap = 1;
            else if (k == 1)           /* p ≡ 1 or 4 (mod 5) */
                ap = (seq_t)(e + 1);
            else                       /* k == -1   (p ≡ 2 or 3 (mod 5)) */
                ap = ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;

            res *= ap;
            if (res == 0) return 0;    /* early exit */
        }
    }

    if (x > 1)  /* remaining prime factor */
    {
        int k = kronecker20(x);
        seq_t ap;
        if (k == 0)
            ap = 1;
        else if (k == 1)
            ap = (seq_t)2;            /* exponent 1 → e+1 = 2 */
        else                       /* k == -1 → exponent 1 odd → 0 */
            ap = 0;

        res *= ap;
        if (res == 0) return 0;
    }

    return res;
}
