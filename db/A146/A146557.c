/*
 * A146557  Number of collinear triples of distinct points in Zn x Zn with no
 *          two on the same "horizontal" or "vertical" line.
 * https://oeis.org/A146557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n^2/6 * ( n^3 * ∏(1+1/p-1/p^(k+1)) - 6*n*(n-1) * ∏(1 +
 *            k*(p-1)/p) + 7*n^2 - 12*n + 4 )
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the second closed‑form expression
   from OEIS A146557. It uses 128‑bit arithmetic for intermediate
   values so that the result is exact for the 64‑bit build.  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64 function from utils */

seq_t a(idx_t n)
{
    /* the first two terms are 0 */
    if (n < 3)
        return 0;

    /* factorisation of n */
    const size_t MAX_FACTORS = 20;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* integer variables used in 128‑bit arithmetic */
    __int128 D1 = 1;        /* ∏ p^(k+1) */
    __int128 D2 = 1;        /* ∏ p          */
    __int128 ProdPK = 1;    /* ∏ p^k        */
    __int128 N1 = 1;        /* ∏ (p^(k+1)+p^k-1) */
    __int128 N2 = 1;        /* ∏ ((k+1)*p-k)   */

    for (size_t i = 0; i < cnt; ++i) {
        __int128 p = ( __int128 )primes[i];
        uint64_t k = exps[i];

        /* p^k */
        __int128 pow_pk = 1;
        for (uint64_t j = 0; j < k; ++j)
            pow_pk *= p;

        /* p^(k+1) */
        __int128 pow_pk1 = pow_pk * p;

        D1      *= pow_pk1;
        D2      *= p;
        ProdPK  *= pow_pk;
        N1      *= (pow_pk1 + pow_pk - 1);
        __int128 termN2 = ( __int128 )(k + 1) * p - ( __int128 )k;
        N2      *= termN2;
    }

    /* n, n^2, n^3 in 128-bit */
    __int128 n_i   = ( __int128 )n;
    __int128 n_sq  = n_i * n_i;
    __int128 n_cu  = n_sq * n_i;

    /* compute the three parts of the bracket */
    __int128 term1 = n_cu * N1;                            /* n^3 * N1 */
    __int128 term2 = (__int128)6 * n_i * (n_i - 1) * N2 * ProdPK;
    __int128 term3 = ( (__int128)7 * n_sq - (__int128)12 * n_i + (__int128)4 ) * D1;

    __int128 M = term1 - term2 + term3;                      /* numerator of bracket */
    __int128 numer = n_sq * M;
    __int128 denom = (__int128)6 * D1;

    return (seq_t)(numer / denom);
}
