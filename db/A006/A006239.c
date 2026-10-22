/*
 * A006239  Row 3 of array in A212801.
 * https://oeis.org/A006239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 54*a(n-2) + 91*a(n-3) - 49*a(n-4), a(1) = 1,
 *            a(2) = 13, a(3) = 108
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 13;
    if (n == 3) return 108;
    
    seq_t a0 = 0, a1 = 1, a2 = 13, a3 = 108;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a4 = 13 * a3 - 54 * a2 + 91 * a1 - 49 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    
    return a3;
}
