/*
 * A003410  Expansion of (1+x)(1+x^2)/(1-x-x^3).
 * https://oeis.org/A003410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 5
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t vals[] = {1, 2, 3, 5};
    if (n <= 3) return vals[n];
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = vals[3] + vals[1];
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = next;
    }
    
    return vals[3];
}
