/*
 * A163433  Number of different fixed (possibly) disconnected trominoes
 *          bounded tightly by an n X n square.
 * https://oeis.org/A163433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 - 12*n + 4 for n > 1; a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    seq_t nm = (seq_t)n;
    return 6 * nm * nm - 12 * nm + 4;
}
