/*
 * A146084  Expansion of 1/(1-x(1-12x)).
 * https://oeis.org/A146084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 12*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 35, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;

    /* a0 holds a(n-2), a1 holds a(n-1), iterate to a(n). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t next;

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* a(i) = a1 - 12*a0 */
        next = a1 - (seq_t)12 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
