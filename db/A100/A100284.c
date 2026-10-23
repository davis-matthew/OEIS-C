/*
 * A100284  Expansion of (1-4*x-x^2)/((1-x)*(1-4*x-5*x^2)).
 * https://oeis.org/A100284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(3 + 5^n + 2*(-1)^n) Recurrence: a(0)=1, a(1)=1,
 *            a(2)=5; a(n)=5*a(n-1)+a(n-2)-5*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)5;

    /* a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)5;   /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = 5*a(i-1) + a(i-2) - 5*a(i-3) */
        next = (seq_t)5 * a_nm1 + a_nm2 - (seq_t)5 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
