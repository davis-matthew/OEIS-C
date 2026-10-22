/*
 * A003520  a(n) = a(n-1) + a(n-5); a(0) = ... = a(4) = 1.
 * https://oeis.org/A003520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-1) + a(n-5), a(0) = a(1) = a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 81, u64 n <= 160,
 *            u128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 4) return 1;
    
    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1, a4 = 1;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = a4 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    
    return a4;
}
