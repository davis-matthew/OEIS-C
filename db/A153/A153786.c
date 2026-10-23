/*
 * A153786  6 times heptagonal numbers: a(n) = 3*n*(5*n-3).
 * https://oeis.org/A153786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(5*n-3)
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* promote index to seq_t */
    seq_t result = 3 * N * (5 * N - 3);
    return result;
}
