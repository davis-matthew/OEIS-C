/*
 * A005085  Sum of 4th powers of primes = 3 mod 4 dividing n.
 * https://oeis.org/A005085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of p^4 over primes p ≡ 3 (mod 4) dividing n
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Check factor 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Trial division for odd factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 4 == 3) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq * p_seq * p_seq;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // Check if remaining temp > 1
    if (temp > 1) {
        if (temp % 4 == 3) {
            seq_t p_seq = (seq_t)temp;
            result += p_seq * p_seq * p_seq * p_seq;
        }
    }
    
    return result;
}
