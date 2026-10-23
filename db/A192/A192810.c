/*
 * A192810  Coefficient of x^2 in the reduction of the polynomial (x^2 + 2)^n
 *          by x^3 -> x^2 + 2.
 * https://oeis.org/A192810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 12*a(n-2) + 8*a(n-3)
   Initial values: a(0)=0, a(1)=1, a(2)=5 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)5;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)7 * a2 - (seq_t)12 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
