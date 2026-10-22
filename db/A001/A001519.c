/*
 * A001519  a(n) = 3*a(n-1) - a(n-2) for n >= 2, with a(0) = a(1) = 1.
 * https://oeis.org/A001519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, core, changed
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }
    
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = (seq_t)3 * curr - prev;
        prev = curr;
        curr = next;
    }
    
    return curr;
}
