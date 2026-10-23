/*
 * A147796  Number of consistent sets of 3 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147796
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,3) * (n^3 - 5*n - 6)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    seq_t n_t = (seq_t)n;
    seq_t binom = n_t * (n_t - 1) * (n_t - 2) / 6;
    seq_t poly = n_t * n_t * n_t - 5 * n_t - 6;
    return binom * poly;
}
