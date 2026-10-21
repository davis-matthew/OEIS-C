/*
 * A092971  Row 6 of array in A288580.
 * https://oeis.org/A092971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{i=0}^{⌊2n/6⌋} (n - 6*i) , excluding the factor 0 (i.e.
 *            ignore the term when n-6*i == 0). This is the definition of
 *            A092971 (Row 6 of A288580)
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;                /* initial product value */
    idx_t max_i = (n * 2) / 6;           /* floor(2n/6) */

    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t term = (seq_t)n - (seq_t)i * 6;
        if (term != 0)
            prod *= term;
    }
    return prod;
}
