/*
 * A168398  a(n) = 4 + 8*floor((n-1)/2).
 * https://oeis.org/A168398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4 + 8*floor((n-1)/2)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first to avoid
       unnecessary widening or narrowing.  All operations are
       performed in seq_t, which is guaranteed to support the
       required arithmetic for values that fit in it. */
    seq_t x = (seq_t)n;
    /* floor((n-1)/2) is simply integer division after subtraction */
    seq_t half = (x - 1) / 2;
    /* Multiply by 8 and add 4, staying within seq_t. */
    seq_t result = half * (seq_t)8 + (seq_t)4;
    return result;
}
