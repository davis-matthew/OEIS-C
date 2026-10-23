/*
 * A107874  Column 0 of triangle A107873; a(n) = C( n*(n-1)/2 + n+3, n).
 * https://oeis.org/A107874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n*(n+1)/2 + 3, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t N = ((seq_t)n * ((seq_t)n + 1)) / 2 + 3;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = N - n + i;   /* N - (n - i) */
        result = (unsigned __int128)result * numerator / i;
    }
    return result;
}
