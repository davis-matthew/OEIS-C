/*
 * A005084  Sum of cubes of primes = 3 mod 4 dividing n.
 * https://oeis.org/A005084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^3 if p = 3 (mod 4), 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    
    // Trial division by 2
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Trial division by odd numbers
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (p % 4 == 3) {
                result += (seq_t)p * (seq_t)p * (seq_t)p;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1, then m is a prime factor
    if (m > 1 && m % 4 == 3) {
        result += (seq_t)m * (seq_t)m * (seq_t)m;
    }
    
    return result;
}
