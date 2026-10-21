/*
 * A227849  a(n) = 2 * floor( 3/14 * n^2) if n even, a(n) = 2 * round( 3/14 *
 *          n^2) -1 if n odd.
 * https://oeis.org/A227849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 * floor(3/14 * n^2) if n even, a(n) = 2 * round(3/14 *
 *            n^2) - 1 if n odd
 * Range:     i8 n <= 6, i16 n <= 275, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit index. Convert to seq_t so arithmetic
       is performed in the desired integer type. */
    seq_t nn = (seq_t)n;                 /* cast to seq_t */
    seq_t n2 = nn * nn;                 /* n squared */
    seq_t temp = 3 * n2;                /* 3 * n^2 */

    if ((n & 1) == 0)                   /* n even: use floor */
    {
        return 2 * (temp / 14);         /* floor division */
    }
    else                                /* n odd: use round */
    {
        seq_t r = (temp + 7) / 14;      /* round = floor(x + 1/2) */
        return 2 * r - 1;
    }
}
