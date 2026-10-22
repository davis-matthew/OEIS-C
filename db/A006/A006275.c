/*
 * A006275  Pierce expansion of sqrt(2) - 1.
 * https://oeis.org/A006275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; a(2*k+1) = c(k) - 1, a(2*k+2) = c(k) + 1, where c(0) =
 *            6, c(n+1) = c(n)^3 - 3*c(n)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 2;
    }
    
    // Determine which c(k) we need
    idx_t k;
    int is_odd = n & 1;
    if (is_odd) {
        // a(2*k+1) = c(k) - 1
        k = (n - 1) / 2;
    } else {
        // a(2*k) = c(k-1) + 1
        k = n / 2 - 1;
    }
    
    // Compute c(k) using recurrence
    seq_t c = 6;
    for (idx_t i = 0; i < k; i++) {
        c = c * c * c - 3 * c;
    }
    
    // Return c(k) ± 1
    if (is_odd) {
        return c - 1;
    } else {
        return c + 1;
    }
}
