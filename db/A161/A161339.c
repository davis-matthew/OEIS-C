/*
 * A161339  Partial sums of A161205.
 * https://oeis.org/A161339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{n} ( 2*floor(sqrt(k)) - (is_square(k)?1:0) ),
 *            with a(1)=1 (partial sums of the sequence A161205)
 * Range:     u8 n <= 35, u16 n <= 1359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n */
static inline idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;   /* hi > sqrt(n) for any 64‑bit n */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)   /* avoid overflow: mid*mid <= n */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the offset – undefined behavior is acceptable */

    idx_t rmax = isqrt(n);         /* floor(sqrt(n)) */
    seq_t sum = 0;

    /* Sum over full blocks [r², (r+1)²-1] for r = 1 .. rmax-1 */
    for (idx_t r = 1; r < rmax; ++r)
    {
        seq_t R = (seq_t)r;
        seq_t block = 4 * R * R + 2 * R - 1;   /* (2r-1) + 2r·(2r)  */
        sum += block;
    }

    /* Partial block from rmax² up to n */
    idx_t start = rmax * rmax;
    for (idx_t k = start; k <= n; ++k)
    {
        seq_t d = (k == start) ? (seq_t)(2 * rmax - 1) : (seq_t)(2 * rmax);
        sum += d;
    }

    return sum;
}
