/*
 * A006061  Star numbers (A003154) that are squares.
 * https://oeis.org/A006061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*(a(n-1) - a(n-2)) + a(n-3), a(1) = 1, a(2) = 121, a(3)
 *            = 11881
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 121;
    if (n == 3) return 11881;
    
    seq_t a1 = 1, a2 = 121, a3 = 11881;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = (seq_t)99 * (a3 - a2) + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    
    return a3;
}
