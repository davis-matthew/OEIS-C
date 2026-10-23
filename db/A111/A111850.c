/*
 * A111850  Number of numbers m <= n such that 0 equals the first digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of integers m ≤ n for which the first digit after the decimal
   point of sqrt(m) is zero.
   For each integer k = floor(sqrt(m)), the interval
   [k², (k+0.1)²) contains floor(k/5)+1 such numbers.
   The function computes this count in O(1) time. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit integers. */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x, r = 0;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0)
        {
            r = 0;
            lo = 1;
            continue;
        }
        if (mid > x / mid)          /* mid*mid > x */
            hi = mid - 1;
        else
        {
            r = mid;
            lo = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* definition for n < 1 is irrelevant */

    idx_t k = isqrt_idx(n);              /* floor(sqrt(n)) */
    /* Count full intervals for k' = 1 .. k-1 */
    idx_t K = k - 1;
    seq_t sum_floor = 0;
    if (K > 0)
    {
        idx_t q = K / 5;                 /* number of complete groups of 5 */
        idx_t r = K % 5;
        /* sum_{t=1}^{K} floor(t/5) = 5*q*(q-1)/2 + (r+1)*q */
        sum_floor = (seq_t)5 * (seq_t)q * (seq_t)(q - 1) / 2
                    + (seq_t)(r + 1) * (seq_t)q;
    }
    seq_t cnt = (seq_t)(k - 1) + sum_floor;   /* contributions from k' < k */

    /* Partial interval for k = floor(sqrt(n)) */
    idx_t upper_k = k * k + k / 5;            /* floor((k+0.1)²) */
    idx_t upper_limit = (n < upper_k) ? n : upper_k;
    seq_t partial = (seq_t)(upper_limit - k * k + 1); /* at least 1 */

    cnt += partial;
    return cnt;
}
