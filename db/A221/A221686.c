/*
 * A221686  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 153*n^2 - 213*n + 96 for n > 3 a(1) = 64, a(2) = 320,
 *            a(3) = 844
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 64;
    if (n == 2) return 320;
    if (n == 3) return 844;

    seq_t nn = (seq_t)n;
    /* Compute 153*n^2 - 213*n + 96 */
    return ((seq_t)153) * nn * nn - ((seq_t)213) * nn + ((seq_t)96);
}
