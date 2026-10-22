/*
 * A001616  Number of parabolic vertices of Gamma_0(n).
 * https://oeis.org/A001616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = p^[e/2] + p^[(e-1)/2]
 * Range:     u8 n <= 9053, u16 n <= 9053, u32 n <= 9053, u64 n <= 9053,
 *            u128 n <= 9053, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            
            seq_t p_seq = (seq_t)p;
            seq_t term = 1;
            
            for (idx_t i = 0; i < e / 2; i++) {
                term *= p_seq;
            }
            
            seq_t p_pow = 1;
            for (idx_t i = 0; i < (e - 1) / 2; i++) {
                p_pow *= p_seq;
            }
            
            result *= (term + p_pow);
        }
    }
    
    if (m > 1) {
        result *= 2;
    }
    
    return result;
}
