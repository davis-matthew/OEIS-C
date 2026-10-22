/*
 * A001120  a(0) = a(1) = 1; for n > 1, a(n) = n*a(n-1) + (-1)^n.
 * https://oeis.org/A001120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + (-1)^n, a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    
    seq_t curr = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = (seq_t)i * curr;
        if (i % 2 == 0) {
            curr = curr + (seq_t)1;
        } else {
            curr = curr - (seq_t)1;
        }
    }
    
    return curr;
}
