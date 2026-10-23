/*
 * A105579  a(n+3) = 2a(n+2) - 3a(n+1) + 2a(n); a(0) = 1, a(1) = 3, a(2) = 4.
 * https://oeis.org/A105579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 2*a(n+2) - 3*a(n+1) + 2*a(n),
   a(0) = 1, a(1) = 3, a(2) = 4 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = 2*a2 - 3*a1 + 2*a0;  /* a(i) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
