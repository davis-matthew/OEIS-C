/*
 * A123950  Expansion of g.f.: x^2*(1-2*x) / (1-3*x-3*x^2+2*x^3).
 * https://oeis.org/A123950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 2*a(n-3), a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    seq_t a_nm3 = (seq_t)0;   /* a(1) */
    seq_t a_nm2 = (seq_t)1;   /* a(2) */
    seq_t a_nm1 = (seq_t)1;   /* a(3) */
    seq_t a_n   = a_nm1;      /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)3 * a_nm1 + (seq_t)3 * a_nm2 - (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
