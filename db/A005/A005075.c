/*
 * A005075  Sum of squares of primes = 2 mod 3 dividing n.
 * https://oeis.org/A005075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Additive with a(p^e) = p^2 if p = 2 (mod 3), 0 otherwise. */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            if (p % 3 == 2) {
                result += (seq_t)p * (seq_t)p;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1 && temp % 3 == 2) {
        result += (seq_t)temp * (seq_t)temp;
    }
    
    return result;
}
