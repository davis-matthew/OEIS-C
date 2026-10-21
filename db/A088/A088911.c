/*
 * A088911  Period 6: repeat [1, 1, 1, 0, 0, 0].
 * https://oeis.org/A088911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+3) mod 6) / 3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n % 6) is 0,1,2 → a(n)=1; else 0 */
    return (seq_t)((n % 6) < 3 ? 1 : 0);
}
