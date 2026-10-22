/*
 * A004018  Theta series of square lattice (or number of ways of writing n as
 *          a sum of 2 squares). Often denoted by r(n) or r_2(n).
 * https://oeis.org/A004018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = 4*product((1+exp_p) for primes p≡1 (mod 4)), 0 if any
 *            prime p≡3 (mod 4) has odd exponent; a(0)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 4;
    uint64_t temp = n;
    
    // Remove factor of 2
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Factor out odd primes
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        uint64_t exp = 0;
        while (temp % p == 0) {
            exp++;
            temp /= p;
        }
        
        if (exp > 0) {
            if (p % 4 == 3) {
                if (exp % 2 == 1) {
                    return 0;
                }
            } else {
                result *= (seq_t)(exp + 1);
            }
        }
    }
    
    // Handle remaining prime factor > sqrt(n)
    if (temp > 1) {
        if (temp % 4 == 3) {
            return 0;
        } else {
            result *= 2;
        }
    }
    
    return result;
}
