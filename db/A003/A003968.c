/*
 * A003968  Möbius transform of A003959.
 * https://oeis.org/A003968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e in factorization of n:
 *            p*(p+1)^(e-1)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
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
            seq_t p_seq = (seq_t)p;
            seq_t term = p_seq;
            for (idx_t i = 1; i < e; i++) {
                term *= (p_seq + 1);
            }
            result *= term;
        }
    }
    
    if (temp > 1) {
        result *= (seq_t)temp;
    }
    
    return result;
}
