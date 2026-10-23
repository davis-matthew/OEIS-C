/*
 * A142720  A triangle sequence of coefficients of odd sum polynomials: p(x,n)
 *          = x^(2*n-1) - (Sum_{i=0..n-1} x^(2*i+1)) - 1.
 * https://oeis.org/A142720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign, tabf
 * Formula:   a(n) is the flattened row‑major list of coefficients of the
 *            odd–sum polynomial p(x,n) = x^(2n-1) - (∑_{i=0}^{n-1} x^(2i+1))
 *            - 1 . Each row n (n≥2) contains 2n‑2 coefficients for the
 *            exponents e = 0 … 2n‑3 in ascending order. The coefficient
 *            equals -1 for exponent 0 or any odd exponent, and 0 for...
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: the very first term is always -1. */
    if (n == 1)
        return (seq_t)-1;

    /* Find the row that contains the nth element.
       The cumulative number of terms up to row k is
          cum(k) = k^2 - k + 1   (k ≥ 1)            */
    idx_t k = 2;                 /* start checking from row 2          */
    idx_t cumulative = 1;         /* cum(1) = 1                         */

    while (1) {
        idx_t row_len = 2 * k - 2;        /* length of row k (k ≥ 2)        */
        if (cumulative + row_len >= n)
            break;
        cumulative += row_len;
        ++k;
    }

    /* Position of n within this row (1‑based). */
    idx_t pos = n - cumulative;
    idx_t exp = pos - 1;      /* exponent corresponding to this position */

    /* Coefficient rule: -1 for exponent 0 or odd exponents, else 0. */
    if (exp == 0 || (exp & 1))
        return (seq_t)-1;
    else
        return (seq_t)0;
}
