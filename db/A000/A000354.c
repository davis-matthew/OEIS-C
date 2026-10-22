/*
 * A000354  Expansion of e.g.f. exp(-x)/(1-2*x).
 * https://oeis.org/A000354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*a(n-1) + (-1)^n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;  // a(0) = 1
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t factor = (seq_t)2 * (seq_t)i;
        result = result * factor;
        if (i & 1) {
            result -= 1;
        } else {
            result += 1;
        }
    }
    
    return result;
}
