/*
 * A035604  Number of points of L1 norm 10 in cubic lattice Z^n.
 * https://oeis.org/A035604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{min(n,10)} C(n,k) * C(9,k-1) * 2^k
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 20, u64 n <= 191, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    uint64_t maxk = n < 10 ? n : 10;

    for (uint64_t k = 1; k <= maxk; ++k)
    {
        /* binom(n,k) */
        seq_t bin_nk = (seq_t)1;
        for (uint64_t i = 1; i <= k; ++i)
            bin_nk = bin_nk * (seq_t)(n - k + i) / (seq_t)i;

        /* binom(9,k-1) */
        uint64_t kk = k - 1;
        seq_t bin_9 = (seq_t)1;
        for (uint64_t i = 1; i <= kk; ++i)
            bin_9 = bin_9 * (seq_t)(9 - kk + i) / (seq_t)i;

        /* 2^k */
        seq_t pow2k = (seq_t)1;
        for (uint64_t i = 0; i < k; ++i)
            pow2k = pow2k * (seq_t)2;

        result = result + bin_nk * bin_9 * pow2k;
    }

    return result;
}
