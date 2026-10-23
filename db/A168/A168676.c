/*
 * A168676  Coefficients of characteristic polynomials for a two diagonal
 *          Matrix type that has determinant equal to trace:M(n)=Table[If[ k
 *          == m && m < n, 1, If[k == m + 1, 1, If[k == 1 && m == n, (-1)^(n +
 *          1)*(n - 1), 0]]], {k, n}, {m, n}].
 * https://oeis.org/A168676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = [m-2] if j = 0; a(n) = (-1)^j * C(m-2, j-1) otherwise
 *            where m = smallest integer with m(m+1)/2 > n, j = n - m(m-1)/2
 *            (0 ≤ j < m). This is the signed coefficient of x^j in the
 *            characteristic polynomial of M_{m-1}. This formula reproduces
 *            the OEIS sequence A168676 for all n ≥ 0
 * Range:     i8 n <= 48, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Find the block index m.  The block m starts at
       start = m*(m-1)/2 and contains m terms.
       We want the smallest m with start <= n < start + m
       which is equivalent to m*(m+1)/2 > n.  */
    idx_t m = 1;
    while (m * (m + 1) / 2 <= n)
        ++m;

    idx_t start = m * (m - 1) / 2;
    idx_t j = n - start;          /* 0 <= j < m */

    /* j == 0 : the first element of the block */
    if (j == 0) {
        if (m >= 2)
            return (seq_t)(m - 2);      /* m-2 is non‑negative for m>=2 */
        else
            return (seq_t)-1;           /* m == 1 gives a(0) = -1 */
    }

    /* j >= 1 : use binomial coefficient */
    idx_t k = j - 1;                   /* k is 0-based */
    seq_t bin = binom(m - 2, k);       /* C(m-2, j-1) */

    if (j & 1)                         /* odd j → sign negative */
        return -bin;
    else
        return bin;
}
