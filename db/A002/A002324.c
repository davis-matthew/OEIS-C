/*
 * A002324  Number of divisors of n == 1 (mod 3) minus number of divisors of n
 *          == 2 (mod 3).
 * https://oeis.org/A002324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, mult
 * Formula:   multiplicative function where a(p^e) depends on p mod 3 and e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t m = n;
    
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            
            seq_t factor;
            if (p % 3 == 0) {
                factor = 1;
            } else if (p % 3 == 1) {
                factor = (seq_t)(e + 1);
            } else {
                factor = (e % 2 == 0) ? 1 : 0;
            }
            
            result *= factor;
            if (result == 0) return 0;
        }
    }
    
    if (m > 1) {
        idx_t p = m;
        seq_t factor;
        if (p % 3 == 0) {
            factor = 1;
        } else if (p % 3 == 1) {
            factor = 2;
        } else {
            factor = 0;
        }
        result *= factor;
    }
    
    return result;
}
