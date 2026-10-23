/*
 * A194333  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n, r=2-phi, where
 *          phi=(1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A194333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i in [1..r] such that the fractional part of *
 *            i*(2-φ) lies in the interval [ (c-1)/r , c/r ), where r is the
 *            row * index determined by n and c is the column index. The code
 *            computes * the row and column from the flattened index n and
 *            then counts the * fractional parts directly u...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static const long double r_val = 2.0L - (1.0L + sqrtl(5.0L)) / 2.0L;   /* r = 2 - φ */

seq_t a(idx_t n)
{
    /* ---- find row r such that r*(r+1)/2 >= n > (r-1)*r/2 ---- */
    long double disc = sqrtl(1.0L + 8.0L * (long double)n);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);

    /* correct rounding errors */
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n) ++r;
    while ( r > 0 && ((unsigned __int128)(r - 1) * r) / 2 >= n) --r;

    idx_t prev = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
    idx_t c = n - prev;          /* column index 1-based */

    /* ---- count fractional parts in the interval ---- */
    long double a_low = (long double)(c - 1) / (long double)r;
    long double a_high = (long double)c / (long double)r;
    const long double eps = 1e-18L;

    seq_t cnt = 0;
    for (idx_t i = 1; i <= r; ++i) {
        long double v = (long double)i * r_val;
        long double frac = v - floorl(v);
        if (frac + eps >= a_low && frac < a_high - eps) {
            ++cnt;
        }
    }
    return cnt;
}
