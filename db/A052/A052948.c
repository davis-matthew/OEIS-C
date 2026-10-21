/*
 * A052948  Expansion of g.f.: (1-2*x)/(1-3*x+2*x^3).
 * https://oeis.org/A052948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3) Initial values: a(0)=1, a(1)=1,
 *            a(2)=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t next = (seq_t)3 * a2 - (seq_t)2 * a0;

        /* Slide the window forward for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
