/*
 * A003412  From a nim-like game.
 * https://oeis.org/A003412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6) for n >= 12, with initial values
 *            a(0)..a(11)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 84, u64 n <= 173,
 *            u128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[12] = {1, 2, 3, 4, 6, 8, 11, 14, 18, 24, 32, 43};
    
    if (n < 12) {
        return init[n];
    }
    
    seq_t buf[6];
    for (int i = 0; i < 6; i++) {
        buf[i] = init[i + 6];
    }
    
    for (idx_t i = 12; i <= n; i++) {
        seq_t next = buf[5] + buf[0];
        for (int j = 0; j < 5; j++) {
            buf[j] = buf[j + 1];
        }
        buf[5] = next;
    }
    
    return buf[5];
}
