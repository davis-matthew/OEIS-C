/*
 * A194756  Number of k such that {-k*Pi} < {-n*Pi}, where { } = fractional
 *          part.
 * https://oeis.org/A194756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For i = floor((n-1)/7) and start = 7*i + 1, a(n) = start - (n -
 *            start) * i. This yields a(1)=1, a(8)=8, a(15)=15, a(22)=22, etc
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index i = floor((n-1)/7) */
    idx_t i = (n - 1) / 7;

    /* start of the block: 7*i + 1 */
    seq_t start = (seq_t)7 * (seq_t)i + 1;

    /* distance from the start of the block */
    seq_t diff = (seq_t)n - start;

    /* a(n) = start - i * diff */
    seq_t result = start - (seq_t)i * diff;

    return result;
}
