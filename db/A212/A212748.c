/*
 * A212748  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          w=2*floor((x+y+z)/2).
 * https://oeis.org/A212748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 9*n^2 + 26*n + 6)/6 for n even; a(n) = (n^3 +
 *            6*n^2 + 11*n + 6)/6 for n odd
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_se = (seq_t)n;
    seq_t num;
    if ((n & 1ULL) == 0ULL) {
        num = n_se * n_se * n_se
            + (seq_t)9 * n_se * n_se
            + (seq_t)26 * n_se
            + (seq_t)6;
    } else {
        num = n_se * n_se * n_se
            + (seq_t)6 * n_se * n_se
            + (seq_t)11 * n_se
            + (seq_t)6;
    }
    return num / (seq_t)6;
}
