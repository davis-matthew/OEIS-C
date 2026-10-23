/*
 * A100967  Least k such that binomial(2k + 1, k - n) >= binomial(2k, k).
 * https://oeis.org/A100967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : binom(2k+1, k-n) >= binom(2k,k)} (Offset 1)
 * Range:     u8 n <= 12, u16 n <= 212, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   cond
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/cond.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* offset handling, not used in tests */
        return 0;

    idx_t lo = n;
    /* Upper bound: 2*n*n + n is safely above the asymptotic value
       a(n) ~ (n^2 + n)/log2.  */
    unsigned __int128 tmp = (unsigned __int128)2 * n * n + n;
    idx_t hi = (idx_t)tmp;
    if (hi < lo)
        hi = lo;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cond(mid, n))
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
