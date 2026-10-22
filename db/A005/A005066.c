/*
 * A005066  Sum of squares of odd primes dividing n.
 * https://oeis.org/A005066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   additive with a(p^e) = 0 if p = 2, p^2 otherwise
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    // Remove all factors of 2
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check odd divisors starting from 3
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t p_seq = (seq_t)p;
            result += p_seq * p_seq;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) {
        seq_t temp_seq = (seq_t)temp;
        result += temp_seq * temp_seq;
    }
    
    return result;
}
