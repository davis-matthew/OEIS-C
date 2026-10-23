/*
 * A165795  Array A(n, k) = numerator of 1/n^2 - 1/k^2 with A(0,k) = 1 and
 *          A(n,0) = -1, read by antidiagonals.
 * https://oeis.org/A165795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, tabl, easy, sign
 * Formula:   a(n) is the reduced numerator of 1/i² – 1/j² where (i,j) are
 *            obtained by enumerating the array A(i,j) (A(0,k)=1, A(n,0)=-1)
 *            along antidiagonals
 * Range:     i8 n <= 30, i16 n <= 449, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include gcd utility */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal m such that the nth term lies on it.
       Number of terms up to diagonal m is (m+1)(m+2)/2. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    /* Offset within the diagonal: 0 … m.  Position is (i,j) with
       i = m - offset, j = offset (since diagonal is listed from (m,0) to (0,m)). */
    idx_t prev_terms = m * (m + 1) / 2;   /* terms before this diagonal   */
    idx_t k = n - prev_terms;            /* offset within the diagonal   */
    idx_t i = m - k;                     /* row                           */
    idx_t j = k;                         /* column                        */

    /* Handle border values directly. */
    if (i == 0)          /* A(0,j) = 1 for all j */
        return (seq_t)1;
    if (j == 0)          /* A(n,0) = -1 for all n>0 */
        return (seq_t)-1;

    /* Compute reduced numerator of (1/i²) – (1/j²). */
    seq_t si = (seq_t)i * i;           /* i²    */
    seq_t sj = (seq_t)j * j;           /* j²    */
    seq_t num = sj - si;               /* numerator before reduction */
    seq_t den = si * sj;               /* denominator before reduction */

    /* Reduce the fraction by dividing by the gcd. */
    seq_t abs_num = num >= 0 ? num : -num;
    seq_t g = gcd(abs_num, den);
    return num / g;                    /* may be negative */
}
