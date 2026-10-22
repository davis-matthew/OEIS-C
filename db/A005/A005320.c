/*
 * A005320  a(n) = 4*a(n-1) - a(n-2), with a(0) = 0, a(1) = 3.
 * https://oeis.org/A005320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;
    
    seq_t a0 = 0;  // a(i-2)
    seq_t a1 = 3;  // a(i-1)
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t a2 = (seq_t)4 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }
    
    return a1;
}
