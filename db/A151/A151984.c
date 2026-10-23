/*
 * A151984  Numbers that are congruent to {0, 1} mod 64.
 * https://oeis.org/A151984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64 * floor((n-1)/2) + ((n-1) mod 2)
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1, OK to subtract 1 */
    idx_t m = n - 1;
    seq_t half = (seq_t)(m / 2);
    seq_t odd  = (seq_t)(m % 2);
    return half * (seq_t)64 + odd;
}
