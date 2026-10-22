/*
 * A005091  Number of distinct primes = 3 mod 4 dividing n.
 * https://oeis.org/A005091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes ≡ 3 (mod 4) dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t m = n;
    
    // Remove all factors of 2 (2 ≡ 2 mod 4, doesn't contribute)
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check odd potential prime factors
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            // Remove all factors of p
            while (m % p == 0) {
                m /= p;
            }
            // If p ≡ 3 (mod 4), increment count
            if (p % 4 == 3) {
                count++;
            }
        }
    }
    
    // If m > 1 at this point, m is a prime factor
    if (m > 1 && m % 4 == 3) {
        count++;
    }
    
    return count;
}
