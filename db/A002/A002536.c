/*
 * A002536  a(n) = 8*a(n-2) - 9*a(n-4).
 * https://oeis.org/A002536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-2) - 9*a(n-4); a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 5 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 5;
    
    seq_t v0 = 0, v1 = 1, v2 = 1, v3 = 5;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t v = (seq_t)8 * v2 - (seq_t)9 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v;
    }
    
    return v3;
}
