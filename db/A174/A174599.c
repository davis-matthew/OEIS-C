/*
 * A174599  Triangle T(n, k) = A154646(n,k) - A154646(n,0) + 1, 0 <= k <= n.
 * https://oeis.org/A174599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten row‑major triangle T(n,k) where T(n,k) = f(n,k) -
 *            2^n, f(n,k) = Σ_{j=0}^{k} (-1)^j · C(n+1,j) · [ (3(k-j)+1)^n +
 *            (3(k-j)+2)^n ]. Each row n has n+1 entries, first and last equal
 *            to 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 140,
 *            u128 n <= 362, big any n
 * Helpers:   _floor_isqrt_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Map the flattened index to row n (call it r) and column k. */
    idx_t t  = 8 * n + 1;
    idx_t sq = _floor_isqrt_u64(t);
    idx_t r  = (sq - 1) / 2;
    idx_t k  = n - r * (r + 1) / 2;

    /* Base case: the first row. */
    if (r == 0)
    {
        return (seq_t)1;
    }

    /* 2^r */
    seq_t pow2 = seq_pow((seq_t)2, r);

    /* Compute f(r,k). */
    seq_t sum   = 0;
    seq_t binom = 1;                /* C(r+1,0) */
    for (idx_t j = 0; j <= k; ++j)
    {
        idx_t d   = k - j;          /* distance from the end of the row */
        seq_t p1  = seq_pow((seq_t)(3 * d + 1), r);
        seq_t p2  = seq_pow((seq_t)(3 * d + 2), r);
        seq_t term = p1 + p2;
        term *= binom;

        if (j & 1U)
        {
            sum -= term;
        }
        else
        {
            sum += term;
        }

        /* Update binomial coefficient for the next j. */
        if (j < k)
        {
            seq_t num = binom * (seq_t)(r + 1 - j);
            binom = num / (seq_t)(j + 1);
        }
    }

    /* T(r,k) = f(r,k) - 2^r. */
    return sum - pow2;
}
