/*
 * A016865  a(n) = (5n+1)^5.
 * https://oeis.org/A016865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+1)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 16, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence integer type first */
    seq_t x = (seq_t)n * 5 + 1;
    /* Compute x^5 by repeated multiplication */
    seq_t result = x;
    for (int i = 1; i < 5; ++i) {
        result *= x;
    }
    return result;
}
