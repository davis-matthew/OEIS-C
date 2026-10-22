/*
 * A003966  Möbius transform of A003958.
 * https://oeis.org/A003966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Möbius transform of A003958, multiplicative with a(p^e) =
 *            (p-2)*(p-1)^(e-1)
 * Range:     u8 n <= 262, u16 n <= 9898, u32 n <= 9898, u64 n <= 9898,
 *            u128 n <= 9898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 2 == 0) {
        return 0;
    }
    
    if (n == 1) {
        return 1;
    }
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t p_val = (seq_t)p;
            seq_t factor = (p_val - 2);
            for (idx_t i = 1; i < e; i++) {
                factor *= (p_val - 1);
            }
            result *= factor;
        }
    }
    
    if (temp > 1) {
        seq_t p_val = (seq_t)temp;
        seq_t factor = (p_val - 2);
        result *= factor;
    }
    
    return result;
}
