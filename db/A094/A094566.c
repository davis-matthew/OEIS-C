/*
 * A094566  Triangle of binary products of Fibonacci numbers.
 * https://oeis.org/A094566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(i) * F(j) where (i, j) are determined from the position
 *            of n in the triangle of binary products of Fibonacci numbers
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/fib.h"

static idx_t
row_of_n(idx_t n)
{
    if (n == 0) return 0;
    // initial approximation using double
    long double estimate = (sqrtl(8.0L * n + 1.0L) - 1.0L) / 2.0L;
    idx_t r = (idx_t)estimate;
    // adjust to find the smallest r such that r*(r+1)/2 >= n
    while (((__int128)r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && (((__int128)(r - 1) * r) / 2) >= n) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // offset starts at 1
    idx_t r = row_of_n(n);           // row number (1‑based)
    // triangular number of previous rows
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos = n - prev_tri - 1;    // 0‑based position inside the row

    idx_t i, j;
    if (r % 2 == 0) { /* even row: r = 2m */
        idx_t m = r / 2;
        if (pos < m) {
            i = 2 * (pos + 1);
            j = 2 * (r - pos - 1);
        } else {
            idx_t offset = pos - m;
            i = 2 * m + 1 + 2 * offset;
            j = 2 * m - 1 - 2 * offset;
        }
    } else { /* odd row: r = 2m + 1 */
        idx_t m = (r - 1) / 2;
        if (pos < m) {
            i = 2 * (pos + 1);
            j = 2 * (r - 1 - pos);
        } else if (pos == m) {
            i = r;
            j = r;
        } else { /* pos > m */
            /* pos ranges from m+1 to 2m */
            i = 2 * pos + 1;
            j = 2 * r - 1 - 2 * pos;
        }
    }

    seq_t fi = fib(i);
    seq_t fj = fib(j);
    return fi * fj;
}
