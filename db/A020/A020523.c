/*
 * A020523  a(n) = 3rd Euler polynomial evaluated at 2^n and multiplied by 4.
 * https://oeis.org/A020523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*8^n - 6*4^n + 1; Initial values: a(0) = -1, a(1) = 9,
 *            a(2) = 161; Linear recurrence: a(n) = 13*a(n-1) - 44*a(n-2) +
 *            32*a(n-3) (n > 2)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 9, i64 n <= 20, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(9);
    if (n == 2) return (seq_t)(161);

    /* Variables for the last three values of the recurrence. */
    seq_t a0 = (seq_t)(-1);   /* a(0) */
    seq_t a1 = (seq_t)(9);    /* a(1) */
    seq_t a2 = (seq_t)(161);  /* a(2) */
    seq_t ai = (seq_t)(0);    /* placeholder for a(n) */

    /* Iterate from 3 up to n using the recurrence relation. */
    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)(13) * a2 - (seq_t)(44) * a1 + (seq_t)(32) * a0;
        /* Shift the window: a0 <- a1, a1 <- a2, a2 <- ai. */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return ai;
}
