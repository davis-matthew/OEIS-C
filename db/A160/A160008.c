/*
 * A160008  Numerator of Hermite(n, 4/25).
 * https://oeis.org/A160008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 4/25) Recurrence: a(0)=1, a(1)=8,
 *            a(n+1)=8*a(n) - 1250*n*a(n-1) All operations are performed using
 *            the type seq_t
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* a(i+1) = 8*a(i) - 1250*i*a(i-1) */
        seq_t term = (seq_t)1250 * (seq_t)i * a0;
        seq_t next = (seq_t)8 * a1 - term;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
