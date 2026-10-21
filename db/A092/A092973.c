/*
 * A092973  Row 8 of array in A288580.
 * https://oeis.org/A092973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{i=0}^{⌊n/4⌋} (n-8*i), skipping any term equal to 0
 *            (this is the row‑8 definition of A288580, the coefficients of
 *            !n!_8)
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 31, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;            /* product accumulator     */
    idx_t limit = n / 4;         /* highest i to include     */

    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t term = (seq_t)n - ((seq_t)8 * (seq_t)i); /* n-8*i          */
        if (term == 0)                  /* skip the zero factor */
            continue;
        result *= term;                 /* multiply into product */
    }

    return result;
}
