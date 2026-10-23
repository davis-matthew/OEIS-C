/*
 * A188821  Number of n X 6 binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 64, a(2) = 441; for n>2, a(n) = 256*n^2 - 384*n + 144
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 64;
    if (n == 2)
        return 441;
    seq_t tn = (seq_t)n;
    return (seq_t)256 * tn * tn - (seq_t)384 * tn + (seq_t)144;
}
