/*
 * A000377  Expansion of f(-q^3) * f(-q^8) * chi(-q^12) / chi(-q) in powers of
 *          q where chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A000377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   Multiplicative with a(0) = 1, a(2^e) = a(3^e) = 1, a(p^e) = e+1
 *            if p ≡ 1,5,7,11 (mod 24), a(p^e) = (1+(-1)^e)/2 if p ≡
 *            13,17,19,23 (mod 24)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    
    seq_t result = (seq_t)1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            
            seq_t contrib = (seq_t)1;
            
            if (p != 2 && p != 3) {
                uint64_t mod = p % 24;
                if (mod == 1 || mod == 5 || mod == 7 || mod == 11) {
                    contrib = (seq_t)(e + 1);
                } else if (mod == 13 || mod == 17 || mod == 19 || mod == 23) {
                    contrib = ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
                }
            }
            
            result = result * contrib;
        }
    }
    
    if (temp > 1) {
        idx_t p = temp;
        seq_t contrib = (seq_t)1;
        
        if (p != 2 && p != 3) {
            uint64_t mod = p % 24;
            if (mod == 1 || mod == 5 || mod == 7 || mod == 11) {
                contrib = (seq_t)2;
            } else if (mod == 13 || mod == 17 || mod == 19 || mod == 23) {
                contrib = (seq_t)0;
            }
        }
        
        result = result * contrib;
    }
    
    return result;
}
