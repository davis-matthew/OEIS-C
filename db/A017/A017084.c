/*
 * A017084  a(n) = (8*n + 1)^8.
 * https://oeis.org/A017084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 8*n + 1 */
    seq_t base = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t result = (seq_t)1;
    /* Multiply base by itself eight times */
    for (int i = 0; i < 8; ++i)
        result *= base;
    return result;
}
