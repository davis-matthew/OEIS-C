/*
 * A005073  Sum of 4th powers of primes = 1 mod 3 dividing n.
 * https://oeis.org/A005073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of p^4 for all primes p | n where p ≡ 1 (mod 3)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 270, u64 n <= 8494,
 *            u128 n <= 8494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Check 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
        // 2 ≡ 2 (mod 3), so don't add
    }
    
    // Trial division by odd numbers
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 3 == 1) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq * p_seq * p_seq;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, it's a prime larger than sqrt(n)
    if (temp > 1) {
        if (temp % 3 == 1) {
            seq_t temp_seq = (seq_t)temp;
            result += temp_seq * temp_seq * temp_seq * temp_seq;
        }
    }
    
    return result;
}
