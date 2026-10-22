/*
 * A001543  a(0) = 1, a(n) = 5 + Product_{i=0..n-1} a(i) for n > 0.
 * https://oeis.org/A001543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 5 + Product_{i=0..n-1} a(i) for n > 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t prod = 1;
    seq_t current = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        prod = prod * current;
        current = 5 + prod;
    }
    
    return current;
}
