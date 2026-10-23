/*
 * A119306  Expansion of (1-4*x)/(1-x*(1-x)^3).
 * https://oeis.org/A119306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 40, i64 n <= 85, i128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A119306: Expansion of (1-4*x)/(1-x*(1-x)^3).
   Recurrence: a(0)=1, a(1)=-3, a(2)=-6, a(3)=6,
   a(n)=a(n-1)-3*a(n-2)+3*a(n-3)-a(n-4) for n>=4. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t) 1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)-6;
    if (n == 3) return (seq_t) 6;

    /* Use a rolling window of the last four computed values. */
    seq_t a0 = (seq_t) 1;   /* a(0) */
    seq_t a1 = (seq_t)-3;   /* a(1) */
    seq_t a2 = (seq_t)-6;   /* a(2) */
    seq_t a3 = (seq_t) 6;   /* a(3) */
    seq_t an;               /* a(k) being computed */

    for (idx_t k = 4; k <= n; ++k) {
        an = a3 - (seq_t)3 * a2 + (seq_t)3 * a1 - a0; /* recurrence */
        a0 = a1;  /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
