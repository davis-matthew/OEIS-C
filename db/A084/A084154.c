/*
 * A084154  Binomial transform of sinh(x)*cosh(sqrt(2)*x).
 * https://oeis.org/A084154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) + 4*a(n-4), with a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=10
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A084154. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)10;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)0;   /* a(0)   */
    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)2;   /* a(2)   */
    seq_t a3 = (seq_t)10;  /* a(3)   */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - 8 * a1 + 4 * a0;  /* recurrence */
        /* Shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
