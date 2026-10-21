/*
 * A053539  a(n) = n*8^(n-1).
 * https://oeis.org/A053539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*8^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = (seq_t)1;
    seq_t eight = (seq_t)8;
    /* compute 8^(n-1) by repeated multiplication */
    for (idx_t i = 1; i < n; ++i) {
        result *= eight;
    }
    /* multiply by n */
    return (seq_t)n * result;
}
