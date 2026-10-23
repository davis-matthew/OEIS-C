/*
 * A152898  Periodic sequence [1,4,0,0,0] of period 5.
 * https://oeis.org/A152898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n%5==0?1:(n%5==1?4:0))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)4;
    return (seq_t)0;
}
