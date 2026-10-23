/*
 * A194297  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n, r=(1+sqrt(3))/2.
 * https://oeis.org/A194297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i (1 ≤ i ≤ row) with fractional part of i*r
 *            lying in [(k-1)/row , k/row), where row is the smallest integer
 *            with row(row+1)/2 ≥ n and k = n - row(row-1)/2. r = (1+√3)/2
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

#define R ((long double)(1.0L + sqrtl(3.0L)) / 2.0L)

seq_t a(idx_t n) {
    /* Find the row number r such that r(r+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Compute the column index k (1-based within the row) */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;      /* 1 <= k <= r */

    /* Count the fractional parts in the interval [(k-1)/r , k/r) */
    seq_t count = 0;
    long double left  = (long double)(k - 1) / (long double)r;
    long double right = (long double)k / (long double)r;

    for (idx_t i = 1; i <= r; ++i) {
        long double val  = (long double)i * R;
        long double frac = val - floorl(val);
        if (frac >= left && frac < right)
            ++count;
    }
    return count;
}
