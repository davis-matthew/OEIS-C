/*
 * A074728  a(n) = Sum_{k=1..n} k^(n-k)*binomial(n,k-1).
 * https://oeis.org/A074728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(n-k) * binomial(n,k-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;          /* bin = C(n,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k^(n-k) */
        seq_t pw = ipow_idx((seq_t)k, n - k);

        /* accumulate term */
        sum += bin * pw;

        /* update bin to C(n,k) for next iteration */
        if (k < n)
        {
            /* bin = bin * (n - (k-1)) / k  */
            bin = (bin * (seq_t)(n - (k - 1))) / (seq_t)k;
        }
    }
    return sum;
}
