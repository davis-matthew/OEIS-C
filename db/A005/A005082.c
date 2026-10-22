/*
 * A005082  Sum of primes = 3 mod 4 dividing n.
 * https://oeis.org/A005082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p if p ≡ 3 (mod 4), 0 otherwise
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    idx_t temp = n;
    
    // Check for factor 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Check for odd factors
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            if (i % 4 == 3) {
                result += (seq_t)i;
            }
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    
    // temp is now either 1 or a prime factor
    if (temp > 1 && temp % 4 == 3) {
        result += (seq_t)temp;
    }
    
    return result;
}
