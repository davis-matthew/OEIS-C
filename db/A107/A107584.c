/*
 * A107584  a(n) = 4^n - 4*n.
 * https://oeis.org/A107584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 4*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = 1;                     /* 4^0 = 1 */
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;               /* multiply by 4 each step */
    }
    pow4 -= (seq_t)4 * ((seq_t)n);      /* subtract 4*n */
    return pow4;
}
