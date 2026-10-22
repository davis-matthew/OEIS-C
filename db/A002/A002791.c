/*
 * A002791  a(n) = Sum_{d|n, d <= 4} d^2 + 4*Sum_{d|n, d>4} d.
 * https://oeis.org/A002791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= 4} d^2 + 4*Sum_{d|n, d>4} d
 * Range:     u8 n <= 29, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t d_seq = (seq_t)d;
            
            if (d <= 4) {
                result += d_seq * d_seq;
            } else {
                result += 4 * d_seq;
            }
            
            idx_t other = n / d;
            if (other != d) {
                seq_t other_seq = (seq_t)other;
                if (other <= 4) {
                    result += other_seq * other_seq;
                } else {
                    result += 4 * other_seq;
                }
            }
        }
    }
    
    return result;
}
