/*
 * A174605  Partial sums of A011371.
 * https://oeis.org/A174605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 - Sum_{i=0..n} popcount(i)
 * Range:     u8 n <= 24, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum_pop = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* Count set bits in i (works for C and C++) */
        sum_pop += (seq_t)__builtin_popcountll(i);
    }
    seq_t total = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return total - sum_pop;
}
