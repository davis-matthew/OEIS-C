/*
 * A152066  Irregular triangle of polynomial coefficients: p(x,n)=If[n == 0,
 *          x^n - x^Floor[(n - 1)/ 2]*Sum[x^m, {m, 0, n - 2*Floor[(n - 1)/
 *          2]}] + 1/x, x^n - x^Floor[(n - 1)/2]*Sum[x^m, {m, 0, n -
 *          2*Floor[(n - 1)/2]}] + 1].
 * https://oeis.org/A152066
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  tabf, sign, uned
 * Formula:   Sequence a(n) lists the coefficients of the polynomials p(x,n) =
 *            x^n - x^floor((n-1)/2) * Sum_{m=0}^{n-2*floor((n-1)/2)} x^m + 1
 *            (for n ≥ 3). The sequence is obtained by reading the
 *            coefficients of these polynomials from lowest to highest degree,
 *            row by row
 * Range:     i8 n <= 62, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* undefined for n < 3; return 0 */

    /* Convert the global index n into polynomial index m and exponent e. */
    idx_t offset = n - 3;      /* zero‑based offset from the first term */
    idx_t cum = 0;             /* cumulative lengths of already processed rows */
    idx_t m = 3;               /* current polynomial index */

    while (offset >= cum + (m + 1))
    {
        cum += (m + 1);       /* add length of row m */
        m++;                  /* move to next polynomial */
    }

    /* Position within the current polynomial (exponent e). */
    idx_t e = n - (3 + cum);

    /* k = floor((m-1)/2) */
    idx_t k = (m - 1) / 2;

    if (e == 0 || e == m)
        return (seq_t)1;
    else if (e >= k && e <= (m - k))
        return (seq_t)-1;

    return (seq_t)0;
}
