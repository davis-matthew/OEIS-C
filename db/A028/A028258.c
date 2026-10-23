/*
 * A028258  Expansion of 1/((1-2*x)*(1-4*x)(1-8*x)(1-16*x)).
 * https://oeis.org/A028258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 30*a(n-1)-280*a(n-2)+960*a(n-3)-1024*a(n-4), a(0)=1,
 *            a(1)=30, a(2)=620, a(3)=11160
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)620;
    if (n == 3) return (seq_t)11160;
    seq_t a0 = 1, a1 = 30, a2 = 620, a3 = 11160;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)30  * a3;
        seq_t term2 = (seq_t)280 * a2;
        seq_t term3 = (seq_t)960 * a1;
        seq_t term4 = (seq_t)1024 * a0;
        seq_t next  = term1 + term3 - term2 - term4;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
