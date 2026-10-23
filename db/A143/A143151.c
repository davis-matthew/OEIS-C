/*
 * A143151  Triangle read by rows, A051731 * (A020639 * 0^(n-k)), 1<=k<=n.
 * https://oeis.org/A143151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = { lpf(k)   if k divides r
 *                  0        otherwise }
 *
 * Here r is the row number such that r(r+1)/2 ≥ n and k = n − r(r−1)/2,
 * i.e. the position of n within that row (1 ≤ k ≤ r).
 *
 * The entries are the least prime factors of the column index k
 * when k divides the row r; otherwise the entry is 0.  The least
 * prime factor of 1 is defined to be 1.
 */

#include <stdint.h>

static seq_t lpf(idx_t m)
{
    if (m <= 1)
        return (seq_t)1;          /* lpf(1) is defined as 1 */

    /* trial division is fast enough for m ≤ 10⁴ */
    for (idx_t i = 2; i * i <= m; ++i)
        if (m % i == 0)
            return (seq_t)i;     /* i is the smallest prime factor */

    return (seq_t)m;             /* m itself is prime */
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t tri = 1;                /* r(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri += r;                 /* update to r(r+1)/2 */
    }

    /* earlier triangular number T(r-1) */
    idx_t prev_tri = tri - r;
    idx_t k = n - prev_tri;        /* 1 ≤ k ≤ r */

    if (r % k != 0)
        return (seq_t)0;

    return lpf(k);
}
