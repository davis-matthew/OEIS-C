/*
 * A027922  Least k such that 1+2+...+k >= 1^2 + 2^2 + ... + n^2.
 * https://oeis.org/A027922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k(k+1)/2 ≥ n(n+1)(2n+1)/6
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 687, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 987 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined only for n ≥ 1 */

    seq_t n_seq = (seq_t)n;                     /* convert index to seq_t */

    /* Sum of squares S = n(n+1)(2n+1)/6, computed with reduced overflow risk */
    seq_t t = n_seq * (n_seq + 1);              /* n(n+1)  */
    t /= 2;                                     /* divide by 2 */
    seq_t S = t * (2 * n_seq + 1);              /* t(2n+1) */
    S /= 3;                                     /* divide by 3 */

    /* Solve k(k+1)/2 ≥ S  →  k = ceil( (-1 + sqrt(1+8S)) / 2 ) */
    seq_t disc = S * 8 + 1;                     /* 1 + 8S */
    seq_t sq = floor_sqrt(disc);                /* floor(sqrt(disc)) */
    seq_t k = (sq - 1) / 2;                     /* initial floor estimate */

    /* Adjust if the estimate is too low (once or twice at most) */
    while ((k * (k + 1)) / 2 < S)
        ++k;

    return k;
}
