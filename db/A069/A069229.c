/*
 * A069229  a(n) = n*(2^n + 1).
 * https://oeis.org/A069229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^n + 1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;           /* multiply by 2 iteratively to avoid large shifts */
    }
    seq_t nn = (seq_t)n;     /* cast index to seq_t before multiplication */
    return nn * (pow2 + 1);
}
