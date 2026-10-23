/*
 * A143616  Record values in A010371.
 * https://oeis.org/A143616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 6 for n > 4; a(n) = n + 5 for n ≤ 4 (offset 1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return (seq_t)n + 5;   /* 6,7,8,9 */
    else
        return (seq_t)n + 6;
}
