/*
 * A213194  First inverse function (numbers of rows) for pairing function
 *          A211377.
 * https://oeis.org/A213194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*i + j - 1 - (-1)^i + (i + j - 2)*(-1)^t) / 4 where t =
 *            ⌊(-1 + √(8*n-7))/2⌋ i = n - t*(t+1)/2 j = (t*t + 3*t + 4)/2 - n
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit values.
   Returns ⌊√x⌋ without using floating point. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (x >> 1) + 1;
    if (high < 1)
        high = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                 /* floor of the square root */
}

seq_t a(idx_t n)
{
    /* t = floor((-1 + sqrt(8*n-7)) / 2) */
    __int128 temp = (__int128)n * 8 - 7;          /* avoid overflow */
    idx_t t = (isqrt_u64((idx_t)temp) - 1) / 2;

    /* i = n - t*(t + 1)/2 */
    idx_t i = n - t * (t + 1) / 2;

    /* j = (t*t + 3*t + 4)/2 - n */
    idx_t j = (t * t + 3 * t + 4) / 2 - n;

    /* parity signs */
    int sign_i = (i & 1) ? -1 : 1;
    int sign_t = (t & 1) ? -1 : 1;

    /* compute numerator in a signed 64‑bit type */
    long long li = (long long)i;
    long long lj = (long long)j;
    long long num =
        3 * li + lj - 1 - sign_i + ((li + lj - 2) * sign_t);

    return (seq_t)(num / 4);
}
