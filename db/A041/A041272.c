/*
 * A041272  Numerators of continued fraction convergents to sqrt(149).
 * https://oeis.org/A041272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041272: Numerators of continued fraction convergents to sqrt(149)
   Recurrence:  p[-2] = 0, p[-1] = 1
                 p[n] = a[n] * p[n-1] + p[n-2]
   where a[0] = 12, and for n≥1, a[n] follows the period
   [4,1,5,3,3,5,1,4,24] repeating.                                                    */

seq_t a(idx_t n)
{
    /* Periodic coefficients after the initial a[0]=12 */
    static const seq_t period[9] = {
        (seq_t)4, (seq_t)1, (seq_t)5, (seq_t)3, (seq_t)3,
        (seq_t)5, (seq_t)1, (seq_t)4, (seq_t)24
    };

    seq_t prev2 = 0;  /* p[-2] */
    seq_t prev1 = 1;  /* p[-1] */
    seq_t coeff, prod, cur;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Obtain the coefficient a[i] */
        if (i == 0)
            coeff = (seq_t)12;
        else
            coeff = period[(i - 1) % 9u];

        /* Multiply prev1 by coeff using repeated addition to avoid overflow */
        unsigned int m = (unsigned int)coeff;   /* coeff ≤ 24 */
        prod = 0;
        for (unsigned int k = 0; k < m; ++k)
            prod += prev1;

        /* Compute current numerator */
        cur = prod + prev2;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
