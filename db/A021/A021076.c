/*
 * A021076  Expansion of 1/((1-x)(1-2x)(1-4x)(1-6x)).
 * https://oeis.org/A021076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1)-56*a(n-2)+92*a(n-3)-48*a(n-4) with a(0)=1,
 *            a(1)=13, a(2)=113, a(3)=833
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)113;
    if (n == 3) return (seq_t)833;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)13;
    seq_t a2 = (seq_t)113;
    seq_t a3 = (seq_t)833;
    seq_t a4 = 0;
    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)13 * a3 - (seq_t)56 * a2 + (seq_t)92 * a1 - (seq_t)48 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
