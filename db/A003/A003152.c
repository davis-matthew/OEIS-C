/*
 * A003152  A Beatty sequence: a(n) = floor(n*(1+1/sqrt(2))).
 * https://oeis.org/A003152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1 + 1/sqrt(2))) = n + floor(n*sqrt(2)/2)
 * Range:     u8 n <= 15, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    
    // Compute k = floor(n*sqrt(2)/2)
    // Equivalently: largest k such that 2*k^2 <= n^2
    
    // Find upper bound using doubling
    seq_t test = 1;
    while (2 * test * test <= n_sq) {
        test *= 2;
    }
    
    // Binary search for exact value
    seq_t lo = test / 2;
    seq_t hi = test;
    
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (2 * mid * mid <= n_sq) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    
    return n_seq + lo;
}
