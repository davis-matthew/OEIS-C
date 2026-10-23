/*
 * A163434  Number of different fixed (possibly) disconnected tetrominoes
 *          bounded tightly by an n X n square.
 * https://oeis.org/A163434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(n) = (2n^2 -4n +1)*(3n^2 -6n +1) for n>1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 164, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t ns = (seq_t)n;
    seq_t term1 = 2 * ns * ns - 4 * ns + 1;
    seq_t term2 = 3 * ns * ns - 6 * ns + 1;
    return term1 * term2;
}
