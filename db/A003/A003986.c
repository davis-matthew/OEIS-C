/*
 * A003986  Table T(n,k) = n OR k read by antidiagonals.
 * https://oeis.org/A003986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, look
 * Formula:   T(x,y) = x OR y, read by antidiagonals; a(n) = T(p, d-p) where d
 *            is the diagonal and p is the position
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t d = 0;
    idx_t diag_start = 0;
    
    // Find which diagonal n belongs to
    while (diag_start + d + 1 <= n) {
        diag_start += d + 1;
        d++;
    }
    
    // Position within the diagonal
    idx_t p = n - diag_start;
    
    // Coordinates on the diagonal and compute their bitwise OR
    return (seq_t)p | (seq_t)(d - p);
}
