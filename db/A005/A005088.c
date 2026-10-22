/*
 * A005088  Number of primes = 1 mod 3 dividing n.
 * https://oeis.org/A005088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Additive: a(p^e) = 1 if p ≡ 1 (mod 3), 0 otherwise */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t count = 0;
    uint64_t m = (uint64_t)n;
    
    // Remove factors of 2 (2 ≢ 1 mod 3)
    while (m % 2 == 0) m /= 2;
    
    // Remove factors of 3 (3 ≢ 1 mod 3)
    while (m % 3 == 0) m /= 3;
    
    // Check remaining prime factors
    for (uint64_t i = 5; i * i <= m; i += 6) {
        if (m % i == 0) {
            if (i % 3 == 1) count++;
            while (m % i == 0) m /= i;
        }
        
        uint64_t j = i + 2;
        if (m % j == 0) {
            if (j % 3 == 1) count++;
            while (m % j == 0) m /= j;
        }
    }
    
    // If m > 1, then m is a prime
    if (m > 1 && m % 3 == 1) count++;
    
    return count;
}
