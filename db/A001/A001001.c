/*
 * A001001  Number of sublattices of index n in generic 3-dimensional lattice.
 * https://oeis.org/A001001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = s1 * s2 / (p + 1), where s1 =
 *            sum_{i=0}^e p^i and s2 = sum_{i=0}^{e+1} p^i
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
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
        int e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        
        if (e > 0) {
            seq_t p_seq = (seq_t)p;
            seq_t p_plus_1 = p_seq + 1;
            
            // Compute s1 = sum_{i=0}^e p^i
            seq_t s1 = 0;
            seq_t p_pow = 1;
            for (int i = 0; i <= e; i++) {
                s1 += p_pow;
                p_pow *= p_seq;
            }
            
            // Compute s2 = s1 + p^(e+1)
            seq_t s2 = s1 + p_pow;
            
            // a(p^e) = s1 * s2 / (p + 1)
            seq_t term = (s1 * s2) / p_plus_1;
            result *= term;
        }
    }
    
    // Handle remaining prime factor
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        // For prime p: a(p) = p^2 + p + 1
        seq_t term = p * p + p + 1;
        result *= term;
    }
    
    return result;
}
