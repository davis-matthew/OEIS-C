/*
 * A005079  Sum of squares of primes = 1 mod 4 dividing n.
 * https://oeis.org/A005079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive, a(n) = sum of p^2 for all primes p | n with p ≡ 1 (mod
 *            4)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Check divisibility by 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Check odd factors from 3 onwards
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 4 == 1) {
                result += (seq_t)p * (seq_t)p;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1 && temp % 4 == 1) {
        result += (seq_t)temp * (seq_t)temp;
    }
    
    return result;
}
