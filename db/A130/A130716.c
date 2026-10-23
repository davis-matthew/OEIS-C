/*
 * A130716  a(0)=a(1)=a(2)=1, a(n)=0 for n>2.
 * https://oeis.org/A130716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0, 1, 2; a(n) = 0 for n > 2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n < 3) ? (seq_t)1 : (seq_t)0;
}
