/*
 * A221597  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 54*n^2 - 69*n + 63 for n > 2, a(1) = 32, a(2) = 139
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)139;
    seq_t N = (seq_t)n;
    return ((seq_t)54) * N * N - ((seq_t)69) * N + ((seq_t)63);
}
