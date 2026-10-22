/*
 * A001510  a(n) = 2*a(n-1)*(a(n-1)-1) for n > 1, with a(0) = 1, a(1) = 2.
 * https://oeis.org/A001510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2*a(n-1)*(a(n-1)-1) for n > 1, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    seq_t prev = 2;
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = prev * (prev - 1);
        prev = 2 * next;
    }
    return prev;
}
