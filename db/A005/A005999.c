/*
 * A005999  Number of paraffins.
 * https://oeis.org/A005999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(2*n^3 - 6*n^2 + 17*n - 7 - (n+1)*(-1)^n)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1374, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t result = 2*n3 - 6*n2 + 17*ns - 7;
    
    if (n & 1) {
        result += ns + 1;
    } else {
        result -= ns + 1;
    }
    
    return result / 8;
}
