/*
 * A194337  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n, r=3-sqrt(5).
 * https://oeis.org/A194337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A194337 sequence.
   a(n) is the flattened triangular array of g(row, col),
   where g(row, col) counts the number of i in [1..row] such that
   the fractional part of i*(3 - sqrt(5)) lies in
   [ (col-1)/row , col/row ].
   The implementation evaluates the fractional parts with
   long double arithmetic and works for any  n ≥ 1 (n = 0 is
   undefined).  Complexity is O(√n) per call, well below the
   required limits for n up to ~10 000. */

#include <math.h>   /* for sqrtl, floorl */

seq_t a(idx_t n)
{
    /* The irrational constant r = 3 - sqrt(5).  It is
       computed once as a long double for accuracy. */
    static const long double r_const = 3.0L - sqrtl(5.0L);
    const long double eps = 1e-18L;   /* tolerance for comparisons */

    if (n == 0) return 0;   /* undefined but harmless */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = (idx_t)((sqrtl(8.0L * (long double)n - 7.0L) - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (r - 1) * r / 2 >= n) --r;

    /* Column index within the row (1‑based). */
    idx_t col = n - (r - 1) * r / 2;

    /* Pre‑compute the bounds of the interval. */
    long double low  = (long double)(col - 1) / (long double)r;
    long double high = (long double)col          / (long double)r;

    /* Count the number of i in [1..r] whose fractional part of
       i*r_const lies in [low, high]. */
    seq_t count = 0;
    for (idx_t i = 1; i <= r; ++i) {
        long double value = (long double)i * r_const;
        long double frac  = value - floorl(value);
        if (frac + eps >= low && frac <= high + eps) {
            ++count;
        }
    }
    return count;
}
