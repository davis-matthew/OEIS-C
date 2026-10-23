/*
 * A152743  6 times pentagonal numbers: a(n) = 3*n*(3*n-1).
 * https://oeis.org/A152743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(3*n-1)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n * 3;
    return tn * (tn - 1);
}
