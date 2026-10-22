/*
 * A000579  Figurate numbers or binomial coefficients C(n,6).
 * https://oeis.org/A000579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n, 6) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/720
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 122, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    seq_t result = 1;
    for (seq_t i = 1; i <= 6; i++) {
        result = result * ((seq_t)n - i + 1) / i;
    }
    return result;
}
