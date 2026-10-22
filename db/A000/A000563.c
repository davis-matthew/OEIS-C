/*
 * A000563  Number of discordant permutations.
 * https://oeis.org/A000563
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81/40*n^5 - 135/4*n^4 + 1719/8*n^3 - 2487/4*n^2 +
 *            3463/5*n
 * Range:     u8 none, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;
    
    seq_t numerator = 81*x5 - 1350*x4 + 8595*x3 - 24870*x2 + 27704*x;
    return numerator / 40;
}
