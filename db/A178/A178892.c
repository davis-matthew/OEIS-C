/*
 * A178892  a(n) = n OR 5n, where OR is bitwise OR.
 * https://oeis.org/A178892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n OR (5 * n)
 * Range:     u8 n <= 51, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any multiplication to avoid
       overflow in the underlying integer type. */
    seq_t x = (seq_t)n;
    seq_t y = x * (seq_t)5;
    return x | y;
}
