/*
 * A005083  Sum of squares of primes = 3 mod 4 dividing n.
 * https://oeis.org/A005083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^2 if p ≡ 3 (mod 4), 0 otherwise
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Remove factors of 2 (2 is not ≡ 3 mod 4)
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check for odd factors starting from 3
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            // Check if p ≡ 3 (mod 4)
            if (p % 4 == 3) {
                result += (seq_t)p * (seq_t)p;
            }
            // Remove all factors of p
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) {
        if (temp % 4 == 3) {
            result += (seq_t)temp * (seq_t)temp;
        }
    }
    
    return result;
}
