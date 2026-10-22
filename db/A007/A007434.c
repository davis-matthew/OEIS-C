/*
 * A007434  Jordan function J_2(n) (a generalization of phi(n)).
 * https://oeis.org/A007434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, look
 * Formula:   a(n) = Product_{p^e || n} (p^(2e) - p^(2e-2)), multiplicative
 *            function
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    /* Try all potential prime factors */
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            /* Count the power of p in n */
            idx_t e = 0;
            
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            
            /* Compute p^(2e) - p^(2e-2) = p^(2e-2) * (p^2 - 1) */
            seq_t p_seq = (seq_t)p;
            seq_t p_squared = p_seq * p_seq;
            
            /* Compute p^(2e-2) */
            seq_t p_power = 1;
            for (idx_t i = 0; i < 2*e - 2; i++) {
                p_power *= p_seq;
            }
            
            result *= p_power * (p_squared - 1);
        }
    }
    
    /* If temp > 1, then it's a prime factor with e=1 */
    if (temp > 1) {
        seq_t p_seq = (seq_t)temp;
        result *= (p_seq * p_seq - 1);
    }
    
    return result;
}
