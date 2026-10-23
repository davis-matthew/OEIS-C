/*
 * A033412  a(n) = floor(92/n).
 * https://oeis.org/A033412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 92 / n
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset is 1; for n == 0 the behavior does not matter,
       but we return 0 for completeness. */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)92 / (seq_t)n;
}
