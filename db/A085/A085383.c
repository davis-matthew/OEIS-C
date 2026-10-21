/*
 * A085383  Triangle read by rows, 1 <= k <= n: T(n,k) =
 *          floor(n/k)*ceiling(n/k).
 * https://oeis.org/A085383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(r/k) * ceil(r/k) where (r,k) are obtained by
 *            mapping n to the triangle: r = smallest integer with r(r+1)/2 >=
 *            n, k = n - (r-1)r/2
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;          /* n >= 1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* mid*(mid+1)/2 can overflow idx_t; use 128-bit */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* outside defined range */

    idx_t r = find_row(n);
    idx_t prev_terms = (r - 1) * r / 2;   /* number of terms before row r */
    idx_t k = n - prev_terms;            /* column within the row */

    /* compute floor(r/k) and ceil(r/k) in seq_t */
    seq_t f = (seq_t)(r / k);
    seq_t c = (seq_t)((r + k - 1) / k);

    return f * c;
}
