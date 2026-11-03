/*
 * A016825  Positive integers congruent to 2 (mod 4): a(n) = 4*n+2, for n >=
 *          0.
 * https://oeis.org/A016825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, nice, changed
 * Formula:   a(n) = 4*n + 2
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 75 terms through n = 9533 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first to avoid unintended narrowing. */
    return (seq_t)n * 4 + 2;
}
