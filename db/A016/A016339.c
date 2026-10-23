/*
 * A016339  155th cyclotomic polynomial.
 * https://oeis.org/A016339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the 155th cyclotomic polynomial Φ155(x).
 * The polynomial is of degree 120. For n > 120 the coefficient is 0.
 *
 * The coefficients satisfy the relation
 *   sum_{k=0}^{4} a(n-k) = { 1 if n == 0 or n ≡ 0 (mod 31) ≤ 124,
 *                          0 otherwise } .
 * This is solved by a simple recurrence.
 */
seq_t a(idx_t n)
{
    /* Φ155 has degree 120, coefficients beyond that are 0 */
    if (n > 120)
        return (seq_t)0;

    seq_t coeff[121];  /* store coefficients up to degree 120 */

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
        {
            coeff[0] = (seq_t)1;
        }
        else
        {
            seq_t sum = coeff[i - 1];
            if (i >= 2) sum += coeff[i - 2];
            if (i >= 3) sum += coeff[i - 3];
            if (i >= 4) sum += coeff[i - 4];

            /* Right‑hand side of the convolution equation */
            seq_t r = ((i % 31 == 0) && i <= 124) ? (seq_t)1 : (seq_t)0;

            coeff[i] = r - sum;
        }
    }
    return coeff[n];
}
