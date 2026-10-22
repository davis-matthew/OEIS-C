/*
 * A001158  sigma_3(n): sum of cubes of divisors of n.
 * https://oeis.org/A001158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Product_{p^e || n} (p^(3e+3)-1)/(p^3-1) (multiplicative)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            
            /* Compute sigma_3(p^e) = 1 + p^3 + p^6 + ... + p^(3e) */
            seq_t p_seq = (seq_t)p;
            seq_t p3 = p_seq * p_seq * p_seq;
            seq_t term = 1;
            seq_t sigma = 0;
            
            for (idx_t i = 0; i <= e; i++) {
                sigma += term;
                if (i < e) term *= p3;
            }
            
            result *= sigma;
        }
    }
    
    /* Handle remaining prime factor if temp > 1 */
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        seq_t p3 = p * p * p;
        result *= (1 + p3);
    }
    
    return result;
}
