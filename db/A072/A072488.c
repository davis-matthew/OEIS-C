/*
 * A072488  a(1) = 1, a(n) = a(n-1) times largest divisor of n <= n^(1/2).
 * https://oeis.org/A072488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=a(n‑1)*d(n) where d(n) is the largest divisor of n
 *            not exceeding sqrt(n)
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of sqrt for unsigned 64‑bit values */
static idx_t isqrt_u64(idx_t n)
{
    /* Binary search for floor(sqrt(n)) */
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;          /* fits in 64 bits for all mid ≤ 0xFFFFFFFF */
        if (sq == n)
            return mid;
        else if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Largest divisor of n that is ≤ sqrt(n) */
static idx_t largest_divisor_le_sqrt(idx_t n)
{
    idx_t r = isqrt_u64(n);
    for (idx_t d = r; d >= 1; --d)
    {
        if (n % d == 0)
            return d;
    }
    return 1;          /* should never happen for n ≥ 1 */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t result = 1;  /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t d = largest_divisor_le_sqrt(i);
        result = result * (seq_t)d;
    }
    return result;
}
