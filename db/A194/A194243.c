/*
 * A194243  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=2-sqrt(2).
 * https://oeis.org/A194243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊∑_{k=1}^n { (2−√2)·k }⌋ = S_n + n − ⌊√2·n(n+1)/2⌋ − 1
 *            where S_n = ∑_{k=1}^n ⌊√2·k⌋ Since 2−√2 = 2 − √2, we use the
 *            identity { (2−√2)·k } = 1 − { √2·k } and the derivations above
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

# include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t x) is provided */

/* integer square root of a 64‑bit value:
   returns ⌊√x⌋, x must fit in idx_t (64‑bit) and result < 2^32 */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= (unsigned __int128)x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static idx_t max_computed = 0;
    static seq_t *cum_sum = NULL;   /* cum_sum[k] = ∑_{i=1}^{k} ⌊√2·i⌋   */

    /* allocate/grow the table */
    if (n > max_computed) {
        seq_t *tmp = (seq_t *)realloc(cum_sum,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed          */
        cum_sum = tmp;
        if (max_computed == 0) cum_sum[0] = 0;

        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* floor(√2·k) = floor(√(2·k²)) */
            idx_t val64 = isqrt_u64((idx_t)(2ULL * k * k));
            cum_sum[k] = cum_sum[k - 1] + (seq_t)val64;
        }
        max_computed = n;
    }

    /* K = n(n+1)/2  (fits in idx_t for all expected n) */
    idx_t K = (idx_t)n * (n + 1) / 2;

    /* floor(√2·K) = floor(√(2·K²)) */
    idx_t sqrtPart64 = isqrt_u64((idx_t)(2ULL * K * K));

    /* a(n) = S_n + n - floor(√2·K) - 1 */
    seq_t result = cum_sum[n] + (seq_t)n - (seq_t)sqrtPart64 - 1;
    return result;
}
