/*
 * A198265  a(n) = ceiling(n*sqrt(10)).
 * https://oeis.org/A198265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*sqrt(10))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 3295, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the floor of the square root of x, using only integer arithmetic */
static inline seq_t sqrt_floor(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    seq_t r  = 0;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq == x)
        {
            r = mid;
            break;
        }
        if (sq < x)
        {
            r = mid;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t prod = nsq * 10;              /* 10 * n^2 */
    seq_t r = sqrt_floor(prod);         /* floor(n*sqrt(10)) */
    return r + 1;                       /* ceil(n*sqrt(10)) */
}
