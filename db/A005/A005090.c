/*
 * A005090  Number of primes == 2 mod 3 dividing n.
 * https://oeis.org/A005090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = 1 if p ≡ 2 (mod 3), 0 otherwise. Counts
 *            distinct primes p | n where p ≡ 2 (mod 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t count = 0;
    idx_t m = n;
    
    // Check 2
    if (m % 2 == 0) {
        // 2 ≡ 2 (mod 3)
        count++;
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check odd divisors
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 3 == 2) {
                count++;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // Handle remaining prime factor
    if (m > 1 && m % 3 == 2) {
        count++;
    }
    
    return count;
}
