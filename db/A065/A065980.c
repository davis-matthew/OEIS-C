/*
 * A065980  Inverse binomial transform of [1^1,2^2,3^3,...], shifted right by
 *          one index.
 * https://oeis.org/A065980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^{n-k} * C(n-1, k-1) * k^k (shifted
 *            right by one index)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* offset is 1 – n=0 is undefined; return 0 as a placeholder. */
    if (n == 0)
        return 0;

    seq_t sum   = 0;
    seq_t binom = 1;                /* C(n-1,0) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t powk = big_pow((seq_t)k, (seq_t)k);

        /* Sign is + when (n-k) is even, – otherwise. */
        if (((n - k) & 1ULL) == 0ULL)
            sum += binom * powk;
        else
            sum -= binom * powk;

        /* Update binomial coefficient for the next k:
           C(n-1,k) = C(n-1,k-1) * (n-k) / k   (exact division). */
        if (k < n) {
            binom = (binom * (seq_t)(n - k)) / (seq_t)k;
        }
    }
    return sum;
}
