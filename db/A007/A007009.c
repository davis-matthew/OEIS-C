/*
 * A007009  Number of 3-voter voting schemes with n linearly ranked choices.
 * https://oeis.org/A007009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 6*n^3 + 12*n^2 + 8*n)/32 for n even; a(n) = (n^4 +
 *            6*n^3 + 12*n^2 + 10*n + 3)/32 for n odd
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 278, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t numerator;
    
    if (n % 2 == 0) {
        numerator = n4 + 6*n3 + 12*n2 + 8*nn;
    } else {
        numerator = n4 + 6*n3 + 12*n2 + 10*nn + 3;
    }
    
    return numerator / 32;
}
