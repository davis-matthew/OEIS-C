/*
 * A070715  n^7 mod 34.
 * https://oeis.org/A070715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 34
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 34 first to avoid large intermediate values */
    seq_t base = (seq_t)n % 34;
    seq_t result = 1;
    /* Compute base^7 mod 34 by repeated multiplication */
    for (int i = 0; i < 7; ++i)
    {
        result = (result * base) % 34;
    }
    return result;
}
