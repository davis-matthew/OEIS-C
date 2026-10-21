/*
 * A083302  a(n) = (4*9^n + (-1)^n)/5.
 * https://oeis.org/A083302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*9^n + (-1)^n)/5 Recurrence: a(0)=1, a(1)=7,
 *            a(n)=8*a(n-1)+9*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)7; /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)8 + a_nm2 * (seq_t)9;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
