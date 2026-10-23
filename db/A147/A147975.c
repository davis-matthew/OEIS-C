/*
 * A147975  a(n) = 4^n-3^n-2^n-1.
 * https://oeis.org/A147975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4)
   initial values: a(1) = -2, a(2) = -2, a(3) = 2, a(4) = 28 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)(-2);
    if (n == 3) return (seq_t)(2);
    if (n == 4) return (seq_t)(28);

    seq_t a1 = (seq_t)(-2);   /* a(n-4) */
    seq_t a2 = (seq_t)(-2);   /* a(n-3) */
    seq_t a3 = (seq_t)(2);    /* a(n-2) */
    seq_t a4 = (seq_t)(28);   /* a(n-1) */
    seq_t t;

    for (idx_t i = 5; i <= n; ++i)
    {
        t = (seq_t)10 * a4 - (seq_t)35 * a3 + (seq_t)50 * a2 - (seq_t)24 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = t;
    }
    return a4;
}
