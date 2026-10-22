/*
 * A001254  Squares of Lucas numbers.
 * https://oeis.org/A001254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 2*a(n+2) + 2*a(n+1) - a(n), with a(0) = 4, a(1) = 1,
 *            a(2) = 9
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 4;
    if (n == 1) return 1;
    if (n == 2) return 9;
    
    seq_t a0 = 4, a1 = 1, a2 = 9;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a3 = 2 * a2 + 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    
    return a2;
}
