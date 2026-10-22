/*
 * A007431  a(n) = Sum_{d|n} phi(d)*mu(n/d).
 * https://oeis.org/A007431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, mult
 * Formula:   a(n) = Sum_{d|n} phi(d)*mu(n/d) Multiplicative with a(p) = p-2
 *            and a(p^e) = (p-1)^2*p^(e-2) for e > 1
 * Range:     u8 n <= 262, u16 n <= 8202, u32 n <= 8202, u64 n <= 8202,
 *            u128 n <= 8202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
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
            
            seq_t p_seq = (seq_t)p;
            seq_t contribution;
            
            if (e == 1) {
                contribution = p_seq - 2;
            } else {
                seq_t p_minus_1 = p_seq - 1;
                contribution = p_minus_1 * p_minus_1;
                for (idx_t i = 2; i < e; i++) {
                    contribution *= p_seq;
                }
            }
            
            result *= contribution;
        }
    }
    
    if (temp > 1) {
        seq_t p_seq = (seq_t)temp;
        result *= (p_seq - 2);
    }
    
    return result;
}
