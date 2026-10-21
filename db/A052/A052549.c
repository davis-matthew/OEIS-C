/*
 * A052549  a(n) = 5*2^(n-1) - 1, n>0, with a(0)=1.
 * https://oeis.org/A052549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=2*a(n-1)+1 for n>1 (a(1)=4) Equivalent:
 *            a(n)=5*2^(n-1)-1 for n>0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    seq_t val = (seq_t)4;
    for (idx_t i = 2; i <= n; ++i) {
        val = val * (seq_t)2 + (seq_t)1;
    }
    return val;
}
