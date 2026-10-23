/*
 * A192969  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4), a(0)=1, a(1)=2,
 *            a(2)=6, a(3)=12
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;

    /* Rolling computation using only four previous values. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)6;   /* a(n-2) */
    seq_t a3 = (seq_t)12;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* next = 3*a3 - 2*a2 - a1 + a0; */
        next = (seq_t)3 * a3;
        next -= (seq_t)2 * a2;
        next -= a1;
        next += a0;

        /* Shift the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
