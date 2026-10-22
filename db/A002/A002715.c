/*
 * A002715  An infinite coprime sequence defined by recursion.
 * https://oeis.org/A002715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n+1) = 2*a(2n)+1, a(2n) = (a(2n-1)^2-3)/2, a(0) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)3;
    
    for (idx_t i = 1; i <= n; i++) {
        if (i & 1) {
            val = val * (seq_t)2 + (seq_t)1;
        } else {
            val = (val * val - (seq_t)3) / (seq_t)2;
        }
    }
    
    return val;
}
