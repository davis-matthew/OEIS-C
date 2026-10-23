/*
 * A020929  Expansion of (1-4*x)^(17/2).
 * https://oeis.org/A020929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-4)^n * binomial(17/2, n) Recurrence: a(0) = 1; for n ≥
 *            1, a(n) = a(n-1) * ( 2*(2*n-19) ) / n (exact division)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 26, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii  = (seq_t)i;                              /* convert index to seq_t */
        seq_t num = 2 * ((2 * ii) - 19);                   /* numerator of the factor */
        seq_t den = ii;                                     /* denominator of the factor */
        /* res * num / den is an exact integer for all n */
        res = (res * num) / den;
    }
    return res;
}
