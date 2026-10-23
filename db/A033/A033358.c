/*
 * A033358  a(n) = floor(38/n).
 * https://oeis.org/A033358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(38/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n > 38 to avoid unnecessary division */
    if (n > 38u)
        return (seq_t)0;
    return (seq_t)38 / (seq_t)n;
}
