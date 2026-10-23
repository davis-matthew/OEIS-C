/*
 * A180668  a(n) = a(n-1)+a(n-2)+a(n-3)+4*n-8 with a(0)=0, a(1)=0 and a(2)=1.
 * https://oeis.org/A180668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+4*n-8 (a(0)=0, a(1)=0, a(2)=1)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a0 = 0, a1 = 0, a2 = 1;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = a0 + a1 + a2 + (seq_t)4 * (seq_t)i - (seq_t)8;
        a0 = a1;
        a1 = a2;
        a2 = term;
    }
    return a2;
}
