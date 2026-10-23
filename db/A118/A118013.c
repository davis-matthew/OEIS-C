/*
 * A118013  Triangle read by rows: T(n,k) = floor(n^2/k), 1<=k<=n.
 * https://oeis.org/A118013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(r(n)^2 / k(n)) where r(n) is the unique integer
 *            such that r(r-1)/2 < n <= r(r+1)/2 (row index) and k(n) = n -
 *            r(r-1)/2 (column index)
 * Range:     u8 n <= 120, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = 1;
    /* Grow high until we cover n */
    while (high * (high + 1) / 2 < n) {
        high <<= 1;                     /* high *= 2 */
    }
    /* Binary search for minimal r with r(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1, but return 0 */
        return (seq_t)0;
    idx_t r = find_row(n);                       /* row index */
    idx_t prev_total = (r - 1) * r / 2;           /* entries before this row */
    idx_t k = n - prev_total;                    /* column index (1‑based) */
    seq_t rn = (seq_t)r;
    seq_t val = rn * rn / (seq_t)k;              /* floor division */
    return val;
}
