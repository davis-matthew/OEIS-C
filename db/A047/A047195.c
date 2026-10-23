/*
 * A047195  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= n/3.
 * https://oeis.org/A047195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{min(floor(n/3)/2, n - floor(n/3))} *
 *            C(floor(n/3), 2t) * C(n - floor(n/3), t)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* out‑of‑range, return 0 */

    idx_t lo = n / (idx_t)3;                     /* floor(n/3) */
    idx_t hi = n - lo;                           /* remaining elements */

    idx_t max_t = lo / (idx_t)2;                 /* floor(lo/2)  */
    if (max_t > hi) max_t = hi;                  /* t must also fit in high part */

    seq_t sum = (seq_t)0;
    for (idx_t t = (idx_t)1; t <= max_t; ++t)
    {
        seq_t low_part  = binom(lo, (idx_t)2 * t);
        seq_t high_part = binom(hi, t);
        sum = sum + low_part * high_part;
    }
    return sum;
}
