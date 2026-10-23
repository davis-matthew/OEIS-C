/*
 * A199115  a(n) = 5*4^n+1.
 * https://oeis.org/A199115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*4^n + 1, a(0) = 6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;  /* multiply by 4 each iteration */
    }
    return pow4 * (seq_t)5 + (seq_t)1;  /* compute 5*4^n + 1 */
}
