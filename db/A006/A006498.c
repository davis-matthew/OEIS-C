/*
 * A006498  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = a(1) = a(2) = 1, a(3) = 2.
 * https://oeis.org/A006498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = a(1) = a(2) = 1, a(3) =
 *            2
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1 || n == 2) return 1;
    if (n == 3) return 2;
    
    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 2;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = a3 + a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    
    return a3;
}
