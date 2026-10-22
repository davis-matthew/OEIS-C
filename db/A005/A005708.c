/*
 * A005708  a(n) = a(n-1) + a(n-6), with a(i) = 1 for i = 0..5.
 * https://oeis.org/A005708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6), a(0) = ... = a(5) = 1
 * Range:     u8 n <= 25, u16 n <= 47, u32 n <= 91, u64 n <= 179,
 *            u128 n <= 356, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        return 1;
    }
    
    seq_t w[6] = {1, 1, 1, 1, 1, 1};
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t next = w[5] + w[0];
        for (int j = 0; j < 5; j++) {
            w[j] = w[j + 1];
        }
        w[5] = next;
    }
    
    return w[5];
}
