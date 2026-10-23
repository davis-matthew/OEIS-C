/*
 * A019301  Binomial transform of Thue-Morse sequence A010059.
 * https://oeis.org/A019301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^n binom(n,k) for all k with even popcount(k)
 *            (i.e. a(n) = ½ · (2^n + ∑_{k=0}^n binom(n,k)·(-1)^{popcount(k)})
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;          /* result accumulator */
    seq_t bin = 1;          /* binom(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add only for even popcount(k) */
        if ( (__builtin_popcountll(k) & 1) == 0 )
            res += bin;

        if (k == n)          /* last term finished */
            break;

        /* next binomial coefficient: binom(n,k+1) = binom(n,k)*(n-k)/(k+1) */
        bin = bin * (n - k) / (k + 1);
    }
    return res;
}
