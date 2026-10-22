/*
 * A007331  Fourier coefficients of E_{infinity,4}.
 * https://oeis.org/A007331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, mult
 * Formula:   a(n) = Sum_{0<d|n, n/d odd} d^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if ((n / d) % 2 == 1) {
                seq_t d_seq = (seq_t)d;
                result += d_seq * d_seq * d_seq;
            }
            
            if (d * d != n) {
                idx_t other = n / d;
                if (d % 2 == 1) {
                    seq_t other_seq = (seq_t)other;
                    result += other_seq * other_seq * other_seq;
                }
            }
        }
    }
    
    return result;
}
