/*
 * A005065  Sum of 4th powers of primes dividing n.
 * https://oeis.org/A005065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^4
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
        seq_t p = (seq_t)2;
        result += p * p * p * p;
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Check odd factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t p_seq = (seq_t)p;
            result += p_seq * p_seq * p_seq * p_seq;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        result += p * p * p * p;
    }
    
    return result;
}
