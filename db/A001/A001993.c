/*
 * A001993  Number of two-rowed partitions of length 3.
 * https://oeis.org/A001993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n^5 + 225*n^4 + 3160*n^3 + 20475*n^2 + 58629*n +
 *            103680)/103680 + (-1)^n*(n^2+15*n)/256 + ((2*n^2+1) mod 3)*n/27)
 * Range:     u8 none, u16 none, u32 n <= 123, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    
    // Combine all terms with common denominator 103680
    seq_t numerator = 6*n5 + 225*n4 + 3160*n3 + 20475*n2 + 58629*nn + 103680;
    
    // Alternating term: (-1)^n*(n^2+15*n)/256
    // Scaled: 405*(-1)^n*(n^2+15*n)/103680
    seq_t alt_term = 405 * (n2 + 15*nn);
    if (n % 2 == 1) {
        numerator -= alt_term;
    } else {
        numerator += alt_term;
    }
    
    // Modular term: ((2*n^2+1) mod 3)*n/27
    // Scaled: 3840*((2*n^2+1) mod 3)*n/103680
    seq_t mod_val = (2*n2 + 1) % 3;
    numerator += 3840 * mod_val * nn;
    
    return numerator / 103680;
}
