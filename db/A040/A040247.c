/*
 * A040247  Continued fraction for sqrt(264).
 * https://oeis.org/A040247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* continued fraction terms for sqrt(264)
   a(0) = 16
   a(n) = 4 if n is odd, 32 if n is even (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;
    return (n & 1) ? (seq_t)4 : (seq_t)32;
}
