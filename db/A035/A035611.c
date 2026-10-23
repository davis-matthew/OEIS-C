/*
 * A035611  Coordination sequence for lattice D*_22 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{22} 2^k * binom(22,k) * binom(n-1,k-1) + 2^22
 *            * binom((22+2*n)/2 -1, 21) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* special case a(0) */

    const idx_t N = 22;                 /* fixed dimension */
    seq_t result = 0;

    /* first summation part */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = pow2(k);                           /* 2^k */
        term = term * binomial(N, k);                  /* * C(N,k) */
        term = term * binomial(n - 1, k - 1);           /* * C(n-1,k-1) */
        result = result + term;                        /* accumulate */
    }

    /* second term */
    idx_t tnum = (N + 2 * n) / 2 - 1;                   /* ((N+2*n)/2 -1) */
    seq_t term2 = pow2(N) * binomial(tnum, N - 1);      /* 2^N * C(tnum,N-1) */
    result = result + term2;

    return result;
}
