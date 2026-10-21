/*
 * A098212  Expansion of (5-x^2)/((1+x)*(1-6*x+x^2)).
 * https://oeis.org/A098212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=25, a(2)=149; a(n)=5*a(n-1)+5*a(n-2)-a(n-3) for
 *            n>=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)149;

    seq_t a_nm3 = (seq_t)5;
    seq_t a_nm2 = (seq_t)25;
    seq_t a_nm1 = (seq_t)149;
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 5 * a_nm1 + 5 * a_nm2 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
