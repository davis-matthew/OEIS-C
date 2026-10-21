/*
 * A059979  Number of 7-dimensional cage assemblies.
 * https://oeis.org/A059979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,2)^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 33, u128 n <= 798,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t bin = ((seq_t)n * ((seq_t)n + (seq_t)1)) / (seq_t)2;
    seq_t res = bin;
    for (idx_t i = 1; i < 7; ++i) {
        res *= bin;
    }
    return res;
}
