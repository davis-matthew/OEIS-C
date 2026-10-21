/*
 * A091054  Expansion of (1 - 5*x - 2*x^2) / ((1 - x)*(1 + 2*x)*(1 - 6*x)).
 * https://oeis.org/A091054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n + 5*(-2)^n + 4)/10. Recurrence: a(0)=1, a(1)=0,
 *            a(2)=6, a(n)=5*a(n-1)+8*a(n-2)-12*a(n-3) for n>2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    /* We'll iterate using the recurrence.  a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t a2 = (seq_t)6; /* a(2) */
    seq_t a3;            /* a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)5 * a2 + (seq_t)8 * a1 - (seq_t)12 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2; /* a(n) stored in a2 */
}
