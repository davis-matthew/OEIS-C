/*
 * A005089  Number of distinct primes == 1 (mod 4) dividing n.
 * https://oeis.org/A005089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   additive with a(p^e) = 1 if p ≡ 1 (mod 4), 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t m = n;
    
    // Check for factor 2
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check for odd factors starting from 3
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 4 == 1) {
                count++;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1 at this point, m is a prime factor
    if (m > 1 && m % 4 == 1) {
        count++;
    }
    
    return count;
}
