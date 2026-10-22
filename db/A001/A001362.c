/*
 * A001362  Number of ways of making change for n cents using coins of 1, 2,
 *          4, 10 cents.
 * https://oeis.org/A001362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n^3 + 51*n^2 + 3*(-1)^n*(n^2 + 17*n) + 373*n +
 *            1152)/960)
 * Range:     u8 none, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    
    seq_t numerator = 2 * n3 + 51 * n2 + 373 * n_seq + 1152;
    
    // Add alternating term: 3*(-1)^n*(n^2 + 17*n)
    seq_t alt_term = 3 * (n2 + 17 * n_seq);
    if (n & 1) {
        // n is odd: (-1)^n = -1
        numerator -= alt_term;
    } else {
        // n is even: (-1)^n = 1
        numerator += alt_term;
    }
    
    return numerator / 960;
}
