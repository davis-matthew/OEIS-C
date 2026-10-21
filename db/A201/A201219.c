/*
 * A201219  a(1) = 0; for n>1, a(n) = 1 if n is a power of 2, otherwise a(n) =
 *          2.
 * https://oeis.org/A201219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=1 if n is a power of 2 else 2
 * Range:     u8 n <= 9784, u16 n <= 9784, u32 n <= 9784, u64 n <= 9784,
 *            u128 n <= 9784, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    return ((n & (n - 1)) == 0) ? 1 : 2;
}
