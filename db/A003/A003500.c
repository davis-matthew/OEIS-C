/*
 * A003500  a(n) = 4*a(n-1) - a(n-2) with a(0) = 2, a(1) = 4.
 * https://oeis.org/A003500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 2, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    
    seq_t prev = (seq_t)2;
    seq_t curr = (seq_t)4;
    seq_t next;
    
    for (idx_t i = 2; i <= n; i++) {
        next = (seq_t)4 * curr - prev;
        prev = curr;
        curr = next;
    }
    
    return curr;
}
