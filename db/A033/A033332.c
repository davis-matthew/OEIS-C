/*
 * A033332  a(n) = floor(12/n).
 * https://oeis.org/A033332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(12/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; guard against n == 0. */
    if (n == 0)
        return 0;
    return (seq_t)12 / (seq_t)n;
}
