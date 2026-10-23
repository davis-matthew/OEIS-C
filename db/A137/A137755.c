/*
 * A137755  Nontrivial numerators (left to right) of Leibniz's harmonic-like
 *          triangle.
 * https://oeis.org/A137755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabl
 * Formula:   a(n) is the numerator of the interior term in the r‑th row of
 *            Leibniz’s harmonic‑like triangle, where r is the smallest
 *            integer satisfying r*(r-1)/2 ≥ n. For that row the j‑th interior
 *            element (j = n - (r-1)*(r-2)/2) has value sum_{i=0}^{m} C(m,i) /
 *            (r+1-i) with m = min(j, r-j). The return value...
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/gcd_seq.h"

/* Greatest common divisor for seq_t */

seq_t a(idx_t n)
{
    /* Find row r: smallest integer such that r*(r-1)/2 >= n */
    idx_t r = 2;
    while (r * (r - 1) / 2 < n) {
        ++r;
    }

    /* Position of the element in the row (0‑based) */
    idx_t start = (r - 1) * (r - 2) / 2 + 1; /* first index of this row */
    idx_t pos   = n - start;                 /* 0‑based index inside row */
    idx_t j     = pos + 1;                    /* actual column (1‑based) */

    /* m = min(j, r-j) */
    idx_t m = (j < r - j) ? j : (r - j);

    /* Sum the series: sum_{i=0}^{m} C(m,i) / (r+1-i) */
    seq_t num = 0;          /* numerator of running sum */
    seq_t den = 1;          /* denominator of running sum */
    seq_t binom = 1;        /* C(m, i) for current i */

    for (idx_t i = 0; i <= m; ++i) {
        seq_t term_num = binom;
        seq_t term_den = (r + 1) - i;        /* positive denominator */

        /* Add term_num/term_den to the running sum */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;
        seq_t g       = gcd_seq(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        /* Prepare binom for next i: C(m, i+1) = C(m, i)*(m-i)/(i+1) */
        if (i < m) {
            binom = binom * (m - i) / (i + 1);
        }
    }

    /* num/den is in lowest terms; return the numerator */
    return num;
}
