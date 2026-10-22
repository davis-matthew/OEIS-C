/*
 * A005080  Sum of cubes of primes = 1 mod 4 dividing n.
 * https://oeis.org/A005080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive, a(p^e) = p^3 if p ≡ 1 (mod 4), 0 otherwise
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 1636, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Remove all factors of 2 (not ≡ 1 mod 4)
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check odd factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            // p is a prime factor
            if (p % 4 == 1) {
                result += (seq_t)p * (seq_t)p * (seq_t)p;
            }
            // Remove all instances of p
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // temp is either 1 or a prime > sqrt(original temp)
    if (temp > 1 && temp % 4 == 1) {
        result += (seq_t)temp * (seq_t)temp * (seq_t)temp;
    }
    
    return result;
}
