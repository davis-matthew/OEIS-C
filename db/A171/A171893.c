/*
 * A171893  1, 2 and all numbers >= 10.
 * https://oeis.org/A171893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n (for n <= 2), otherwise a(n) = n + 7
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)
        return (seq_t)n;
    else
        return (seq_t)n + (seq_t)7;
}
