/*
 * A033405  a(n) = floor(85/n).
 * https://oeis.org/A033405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊85 / n⌋
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed ≥ 1 per OEIS offset; guard against 0 just in case */
    if (n == 0)
        return 0;
    return (seq_t)85 / (seq_t)n;
}
