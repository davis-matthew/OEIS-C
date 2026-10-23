/*
 * A033584  a(n) = 11*n^2.
 * https://oeis.org/A033584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n^2
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type before any operation
       to preserve exactness for any seq_t width. */
    seq_t x = (seq_t)n;
    /* Multiply first to avoid intermediate 64‑bit overflow where
       seq_t may be larger (e.g., __int128). */
    return (x * x) * (seq_t)11;
}
