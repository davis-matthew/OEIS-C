/*
 * A007203  Number of elements (a b / c d) in GL(2,Z) with |det| = 1, trace n
 *          and 0 <= a <= {b, c} <= d.
 * https://oeis.org/A007203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006546(n) + A007104(n) Count 2x2 integer matrices (a b;
 *            c d) where: trace = a + d = n, |ad - bc| = 1, and 0 ≤ a ≤ b,c ≤
 *            d
 * Range:     u8 n <= 31, u16 n <= 620, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    seq_t count = 0;
    
    for (seq_t a = 0; a < n; a++) {
        seq_t d = n - a;
        seq_t ad = a * d;
        
        // Count matrices with ad - bc = 1
        if (ad > 0) {
            seq_t k = ad - 1;
            for (seq_t b = a; b <= d; b++) {
                if (b > 0 && k % b == 0) {
                    seq_t c = k / b;
                    if (a <= c && c <= d) {
                        count++;
                    }
                }
            }
        }
        
        // Count matrices with ad - bc = -1
        {
            seq_t k = ad + 1;
            for (seq_t b = a; b <= d; b++) {
                if (b > 0 && k % b == 0) {
                    seq_t c = k / b;
                    if (a <= c && c <= d) {
                        count++;
                    }
                }
            }
        }
    }
    
    return count;
}
