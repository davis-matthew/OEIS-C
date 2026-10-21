/*
 * A077903  Expansion of (1-x)^(-1)/(1 + x - x^2 + 2*x^3).
 * https://oeis.org/A077903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-2) - 3*a(n-3) + 2*a(n-4) (for n ≥ 4), with initial
 *            values a(0)=1, a(1)=0, a(2)=2, a(3)=–3
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)(-3);

    /* Keep the last four terms: a_{i-4}, a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t a0 = (seq_t)1;   /* a_{i-4} */
    seq_t a1 = (seq_t)0;   /* a_{i-3} */
    seq_t a2 = (seq_t)2;   /* a_{i-2} */
    seq_t a3 = (seq_t)(-3);/* a_{i-1} */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        /* Compute next term using the recurrence */
        seq_t next = (seq_t)2 * a2 - (seq_t)3 * a1 + (seq_t)2 * a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    /* After loop, a3 holds a(n) */
    return a3;
}
