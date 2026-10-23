/*
 * A016825  Positive integers congruent to 2 (mod 4): a(n) = 4*n+2, for n >=
 *          0.
 * https://oeis.org/A016825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, nice, changed
 * Formula:   a(n) = 4*n + 2
 * Range:     u8 n <= 63, u16 n <= 9533, u32 n <= 9533, u64 n <= 9533,
 *            u128 n <= 9533, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first to avoid unintended narrowing. */
    return (seq_t)n * 4 + 2;
}
