/*
 * A179298  a(n)=n^3-(n-1)^3-(n-2)^3-...-1.
 * https://oeis.org/A179298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^2*(1-6*n+n^2)/4
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* behavior below the offset is irrelevant */

    seq_t nn = (seq_t)n;     /* cast index to seq_t before operations */
    seq_t term = 6*nn - nn*nn - 1;   /* compute (6*n - n^2 - 1) */
    seq_t num  = nn*nn*term;          /* n^2 * term */
    return num / 4;                      /* integer division by 4 */
}
