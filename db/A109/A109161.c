/*
 * A109161  Triangle read by rows: T(n, k) = n*(n+9) + k + 5, with T(0, 0) = 5
 *          and T(1, 0) = 15.
 * https://oeis.org/A109161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = T(row(n), k(n)) where T(n, k) = n*(n+9) + k + 5 row(n) is
 *            the row index such that (row+1)*(row+2)/2 ≥ n and
 *            (row)*(row+1)/2 < n k(n) = n – row(row+1)/2 – 1 (0‑based column
 *            in that row)
 * Range:     u8 n <= 78, u16 n <= 5151, u32 n <= 5151, u64 n <= 5151,
 *            u128 n <= 5151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_of_n(idx_t n)
{
    if (n == 0) return 0;            /* undefined input – return 0 */
    idx_t low = 0;
    idx_t high = 1;
    /* Find an upper bound */
    while (((high + 1) * (high + 2)) / 2 < n)
        high <<= 1;
    /* Binary search the smallest r with (r+1)(r+2)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid + 1) * (mid + 2)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1 – behaviour for n=0 is unspecified */

    idx_t r = row_of_n(n);
    /* number of terms before row r */
    idx_t prev = r * (r + 1) / 2;
    /* column index k in that row */
    idx_t k = n - prev - 1;

    /* compute T(r, k) = r*(r+9) + k + 5 */
    seq_t result = (seq_t)r * ((seq_t)r + 9) + (seq_t)k + (seq_t)5;
    return result;
}
