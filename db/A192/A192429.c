/*
 * A192429  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) - 6*a(n-3) - 9*a(n-4) Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=21 Valid for all n >= 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)21;

    /* Keep the last four terms only. */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)21;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)2 * a3 + (seq_t)10 * a2
                   - (seq_t)6 * a1 - (seq_t)9 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
