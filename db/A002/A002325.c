/*
 * A002325  Glaisher's J numbers.
 * https://oeis.org/A002325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   Multiplicative with a(2^e)=1, a(p^e)=e+1 if p≡1,3(mod 8),
 *            a(p^e)=(1+(-1)^e)/2 if p≡5,7(mod 8)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
            
            seq_t contribution;
            if (p == 2) {
                contribution = 1;
            } else {
                idx_t mod = p % 8;
                if (mod == 1 || mod == 3) {
                    contribution = (seq_t)(e + 1);
                } else {
                    contribution = (e & 1) ? 0 : 1;
                }
            }
            
            result *= contribution;
            if (result == 0) return 0;
        }
    }
    
    if (temp > 1) {
        idx_t p = temp;
        seq_t contribution;
        if (p == 2) {
            contribution = 1;
        } else {
            idx_t mod = p % 8;
            if (mod == 1 || mod == 3) {
                contribution = 2;
            } else {
                contribution = 0;
            }
        }
        result *= contribution;
    }
    
    return result;
}
