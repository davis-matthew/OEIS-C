/*
 * A178393  a(n) = (n^3 - 1)^3 - 1.
 * https://oeis.org/A178393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^3 - 1)^3 - 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = -2, a(1) = -1, ... as defined in OEIS entry A178393 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                       /* Promote the unsigned index to seq_t */
    seq_t t  = tn * tn * tn - 1;                /* Compute n^3 - 1 */
    return t * t * t - 1;                      /* Compute (n^3 - 1)^3 - 1 */
}
