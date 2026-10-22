/*
 * A007104  Number of elements (a b, c d) in GL(2,Z) with det = -1, trace n
 *          and 0 <= a <= {b, c} <= d.
 * https://oeis.org/A007104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count (a,b,c,d) in GL(2,Z) with det(M) = -1, trace(M) = n, 0 <= a <= {b,c} <= d */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t nn = (seq_t)n;
    
    for (seq_t a_val = 0; a_val <= nn; a_val++) {
        seq_t d = nn - a_val;
        seq_t ad_plus_1 = a_val * d + 1;
        
        for (seq_t b = a_val; b <= d; b++) {
            if (b != 0 && ad_plus_1 % b == 0) {
                seq_t c = ad_plus_1 / b;
                if (c >= a_val && c <= d) {
                    count++;
                }
            }
        }
    }
    
    return count;
}
