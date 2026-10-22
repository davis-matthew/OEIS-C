/*
 * A007013  Catalan-Mersenne numbers: a(0) = 2; for n >= 0, a(n+1) = 2^a(n) -
 *          1.
 * https://oeis.org/A007013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^a(n-1) - 1, a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_seq(seq_t e) {
    if (e == 0) return (seq_t)1;
    
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    
    while (e > 0) {
        if (e & 1) {
            result = result * base;
        }
        base = base * base;
        e = e >> 1;
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t result = (seq_t)2;
    
    for (idx_t i = 0; i < n; i++) {
        result = pow2_seq(result) - 1;
    }
    
    return result;
}
