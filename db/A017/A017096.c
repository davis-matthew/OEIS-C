/*
 * A017096  a(n) = (8*n + 2)^8.
 * https://oeis.org/A017096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 8*n + 2 in seq_t */
    seq_t base = (seq_t)n * (seq_t)8 + (seq_t)2;

    /* compute base^8 via repeated multiplication (exact, no overflow in seq_t) */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        result *= base;

    return result;
}
