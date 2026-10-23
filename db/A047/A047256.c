/*
 * A047256  Numbers that are congruent to {0, 1, 2, 3, 5} mod 6.
 * https://oeis.org/A047256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/5) - 1 (OEIS A047256)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t before the arithmetic to avoid truncation on very small
       seq_t types (e.g., uint8_t).  */
    seq_t nn = (seq_t)n;
    seq_t div5 = (seq_t)(n / 5);       /* integer division truncates down */
    return nn + div5 - 1;
}
