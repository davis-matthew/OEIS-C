/*
 * A006276  Pierce expansion of (3 - sqrt(5))/2.
 * https://oeis.org/A006276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   c(0) = 3, c(n+1) = c(n)^3 - 3*c(n); then a(n) = c(floor(n/2)) -
 *            1 + (n mod 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n >> 1;
    
    // Compute c(m) where c(0) = 3, c(n+1) = c(n)^3 - 3*c(n)
    seq_t c = 3;
    for (idx_t i = 0; i < m; i++) {
        seq_t c_sq = c * c;
        c = c_sq * c - 3 * c;
    }
    
    // a(n) = c(floor(n/2)) - 1 if n is even, +1 if n is odd
    if (n & 1) {
        return c + 1;
    } else {
        return c - 1;
    }
}
