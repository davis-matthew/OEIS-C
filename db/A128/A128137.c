/*
 * A128137  A002260 * A128132.
 * https://oeis.org/A128137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = if (k==n) k*k else k*k - k - 1, where n = r*(r-1)/2 + k
 *            with r≥1, 1≤k≤r. The sequence is the flattened lower‑triangular
 *            matrix B(r,k) defined by A128137
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based.  Guard against n==0, though it should never
       be called with n<1. */
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n.  Use a
       floating‑point approximation followed by a small adjustment
       loop to avoid rounding errors. */
    idx_t r = (idx_t)((sqrtl((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while (( (__int128)r * (r + 1) ) / 2 < n) ++r;
    while (r > 0 && ( (__int128)(r - 1) * r ) / 2 >= n) --r;

    /* Number of elements before this row */
    idx_t prev = (idx_t)(( (__int128)(r - 1) * r ) / 2);
    /* Column k (1‑based) within the row */
    idx_t k = n - prev;

    /* Compute the value according to the derived formula. */
    if (r == k) {
        seq_t val = (seq_t)r * (seq_t)r;
        return val;
    } else {
        seq_t kk = (seq_t)k;
        return kk * kk - kk - (seq_t)1;
    }
}
