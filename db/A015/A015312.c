/*
 * A015312  Gaussian binomial coefficient [ n,5 ] for q = -7.
 * https://oeis.org/A015312
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,5] for q = -7 (OEIS
 *            A015312)
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t curr[6] = {0};
    curr[0] = 1;
    seq_t pow[6] = {0};
    seq_t Q = -7;

    for (idx_t m = 1; m <= n; ++m)
    {
        int maxk = (m < 5 ? (int)m : 5);

        /* Update q‑powers for the current n */
        for (int i = maxk; i >= 1; --i)
        {
            if (i == (int)m) pow[i] = 1;
            else pow[i] = pow[i] * Q;        /* q^{m-i} = q * q^{(m-1)-i} */
        }

        /* Update the binomial coefficients descending to keep
           previous values intact for the recurrence */
        for (int i = maxk; i >= 1; --i)
        {
            curr[i] = curr[i] + pow[i] * curr[i-1];
        }
    }

    return curr[5];
}
