/*
 * A158317  a(n) = 400*n - 1.
 * https://oeis.org/A158317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 400*n-1
 * Range:     u8 none, u16 n <= 163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before multiplication to avoid overflow in intermediate. */
    seq_t val = (seq_t)n * (seq_t)400 - (seq_t)1;
    return val;
}
