/*
 * A099515  Row sums of triangle A099514, so that a(n) = Sum_{k=0..n}
 *          coefficient of z^k in (1 + z + 2*z^2)^(n-[k/2]), where [k/2] is
 *          the integer floor of k/2.
 * https://oeis.org/A099515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 3*a(n-3) - 4*a(n-4), with initial
 *            values a(0)=1, a(1)=2, a(2)=5, a(3)=13. This follows from the
 *            rational generating function (1-2*x^2)/(1-2*x-3*x^2+3*x^3+4*x^4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)13;

    /* Variables hold a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a_nm4 = (seq_t)1;  /* a(0) */
    seq_t a_nm3 = (seq_t)2;  /* a(1) */
    seq_t a_nm2 = (seq_t)5;  /* a(2) */
    seq_t a_nm1 = (seq_t)13; /* a(3) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)2 * a_nm1 + (seq_t)3 * a_nm2
              - (seq_t)3 * a_nm3 - (seq_t)4 * a_nm4;

        /* Shift the window forward. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
