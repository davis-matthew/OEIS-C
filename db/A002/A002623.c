/*
 * A002623  Expansion of 1/((1-x)^4*(1+x)).
 * https://oeis.org/A002623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n+3, 3) - a(n-1), with a(0) = 1
 * Range:     u8 n <= 12, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t prev = 1;  // a(0) = 1
    for (idx_t i = 1; i <= n; i++) {
        // C(i+3, 3) = (i+3)*(i+2)*(i+1)/6
        seq_t si = (seq_t)i;
        seq_t binom = (si + 3) * (si + 2) * (si + 1) / 6;
        seq_t curr = binom - prev;
        prev = curr;
    }
    return prev;
}
