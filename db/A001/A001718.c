/*
 * A001718  Generalized Stirling numbers, [n+7,7]_4.
 * https://oeis.org/A001718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^3] Product_{r=0..n+2} (x + 4 + r)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[4] = {1, 0, 0, 0};
    
    for (idx_t r = 0; r <= n + 2; r++) {
        seq_t a_val = 4 + (seq_t)r;
        seq_t new_c[4];
        new_c[0] = c[0] * a_val;
        new_c[1] = c[0] + c[1] * a_val;
        new_c[2] = c[1] + c[2] * a_val;
        new_c[3] = c[2] + c[3] * a_val;
        
        c[0] = new_c[0];
        c[1] = new_c[1];
        c[2] = new_c[2];
        c[3] = new_c[3];
    }
    
    return c[3];
}
