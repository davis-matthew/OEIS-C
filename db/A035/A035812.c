/*
 * A035812  Coordination sequence for lattice D*_54 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..54} 2^k * C(54,k) * C(n-1, k-1) + 2^54 *
 *            C(n+26, 53) (for n ≥ 0, with C(-1,-1)=0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"       /* pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0U)
        return (seq_t)1;

    const idx_t N       = 54U;          /* constant from the definition */
    const idx_t Nminus1 = N - 1U;       /* 53 */
    seq_t result = 0U;

    /* Sum over k = 1 .. N, stopping when k > n (binomial becomes zero) */
    for (idx_t k = 1U; k <= N; ++k)
    {
        if (k > n)          /* binomial(n-1, k-1) will be 0 for larger k */
            break;

        seq_t term = pow2(k);
        term *= binomial(N, k);          /* 2^k * C(54,k) */
        term *= binomial(n - 1U, k - 1U);/* * C(n-1, k-1) */
        result += term;
    }

    /* Second term: 2^54 * C(n+26, 53) */
    seq_t twoN = pow2(N);                  /* 2^54 */
    seq_t extra = twoN * binomial(n + 26U, Nminus1);
    result += extra;

    return result;
}
