/*
 * A127231  a(n) = (2n)! + 1.
 * https://oeis.org/A127231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 2; i <= (idx_t)2 * n; ++i)
        prod *= (seq_t)i;
    return prod + 1;
}
