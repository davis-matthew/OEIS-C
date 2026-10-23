/*
 * A166361  Scale degrees of the roots of chords in a traditional "twelve-bar
 *          blues" in Western music.
 * https://oeis.org/A166361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(n) = pattern[(n-1) mod 12], pattern =
 *            (1,1,1,1,4,4,1,1,5,4,1,1)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {1,1,1,1,4,4,1,1,5,4,1,1};
    idx_t m = (n - 1) % 12;   /* n ≥ 1 by problem statement */
    return pattern[m];
}
