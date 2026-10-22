/*
 * A005081  Sum of 4th powers of primes = 1 mod 4 dividing n.
 * https://oeis.org/A005081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of p^4 for all primes p ≡ 1 (mod 4) dividing n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Check for factor 2
    if (temp % 2 == 0) {
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Trial division for odd numbers
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 4 == 1) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq * p_seq * p_seq;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // Check if remaining temp > 1 (it's a prime)
    if (temp > 1 && temp % 4 == 1) {
        seq_t p_seq = (seq_t)temp;
        result += p_seq * p_seq * p_seq * p_seq;
    }
    
    return result;
}
