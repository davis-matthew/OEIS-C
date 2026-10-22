/*
 * A005071  Sum of squares of primes = 1 mod 3 dividing n.
 * https://oeis.org/A005071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^2 if p = 1 (mod 3), 0 otherwise
 * Range:     u8 n <= 18, u16 n <= 270, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Remove factors of 2 (2 % 3 = 2, not 1)
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check odd numbers starting from 3
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            // p is a prime factor
            if (p % 3 == 1) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq;
            }
            // Remove all powers of p
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1) {
        if (temp % 3 == 1) {
            seq_t temp_seq = (seq_t)temp;
            result += temp_seq * temp_seq;
        }
    }
    
    return result;
}
