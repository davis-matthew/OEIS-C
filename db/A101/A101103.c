/*
 * A101103  Partial sums of A101104. First differences of A005914.
 * https://oeis.org/A101103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=13, a(n)=24*n-36 for n≥3
 * Range:     u8 n <= 12, u16 n <= 2732, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)13;
    return (seq_t)n * 24 - 36;
}
