/*
 * A006528  a(n) = (n^4 + n^2 + 2*n)/4.
 * https://oeis.org/A006528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2-n+2)/4
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t n1 = n_t + 1;
    seq_t n_sq = n_t * n_t;
    seq_t f3 = n_sq - n_t + 2;
    
    // n*(n+1) is always even, f3 is always even
    // Divide by 2 from each: one from n*(n+1), one from f3
    if (n_t & 1) {  // n is odd, so n+1 is even
        return n_t * (n1 / 2) * (f3 / 2);
    } else {  // n is even
        return (n_t / 2) * n1 * (f3 / 2);
    }
}
