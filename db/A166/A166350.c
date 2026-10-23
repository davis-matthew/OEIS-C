/*
 * A166350  Triangle read by rows: T(n,m) = m!, n >= 1.
 * https://oeis.org/A166350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = factorial(c), where c is the column index of n in the
 *            triangle whose rows are 1, 1 2, 1 2 6, 1 2 6 24, …. The row r is
 *            the smallest integer with r*(r+1)/2 >= n. Then c = n - (r-1)*r/2
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute factorial of an idx_t value using seq_t arithmetic. */
static seq_t factorial_idx(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i)
    {
        res *= (seq_t)i;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r) >= n. */
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;                /* double until enough */

    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t t   = (mid * (mid + 1)) / 2;
        if (t < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Column index c */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t c = n - prev_tri;        /* 1 <= c <= r */

    return factorial_idx(c);
}
