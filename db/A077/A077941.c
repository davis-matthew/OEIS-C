/*
 * A077941  Expansion of 1/(1-2*x+x^2+x^3).
 * https://oeis.org/A077941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077941: Expansion of 1/(1-2*x+x^2+x^3)
   Recurrence: a(n) = 2*a(n-1) - a(n-2) - a(n-3)
   Initial values: a(0)=1, a(1)=2, a(2)=3 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)3;   /* a(n-1) */
    seq_t an;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        an = (seq_t)((seq_t)2 * a2 - a1 - a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
