/*
 * A035475  Coordination sequence for lattice D*_12 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..12} 2^k * binomial(12,k) * binomial(n-1,k-1) +
 *            2^12 * binomial((12+2n)/2 - 1 , 11) where (12+2n)/2 - 1 = n + 5.
 *            For n = 0, a(0) = 1 (handled as a special case)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 122, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    const idx_t N = 12;
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    /* First term: sum over k = 1..12 (k = 0 gives binomial(n-1,-1)=0) */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = binomial(N, k);          /* binomial(12,k) */
        term = term * pow2(k);                /* * 2^k */
        term = term * binomial(n - 1, k - 1); /* * binomial(n-1,k-1) */
        sum += term;
    }
    /* Second term: 2^12 * binomial(n+5, 11) */
    seq_t extra = binomial(n + 5, N - 1);     /* binomial(n+5, 11) */
    extra = extra * pow2(N);                  /* * 2^12 */
    sum += extra;

    return sum;
}
