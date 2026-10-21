/*
 * A217200  Number of permutations in S_{n+2} containing an increasing
 *          subsequence of length n.
 * https://oeis.org/A217200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(n) = (n^2 + n^4)/2 + n^3 + n + 3 for n>0
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    /* Use a wider type to avoid intermediate overflow when seq_t is small. */
    unsigned __int128 nt = (unsigned __int128)n;
    unsigned __int128 n2 = nt * nt;
    unsigned __int128 n4 = n2 * n2;
    unsigned __int128 part = (n2 + n4) / (unsigned __int128)2;
    unsigned __int128 n3 = n2 * nt;
    unsigned __int128 result = part + n3 + nt + (unsigned __int128)3;
    return (seq_t)result;
}
