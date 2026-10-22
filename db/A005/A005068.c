/*
 * A005068  Sum of 4th powers of odd primes dividing n.
 * https://oeis.org/A005068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of p^4 for each odd prime p dividing n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    uint64_t m = n;
    
    /* Skip factors of 2 */
    while (m % 2 == 0) {
        m /= 2;
    }
    
    /* Check odd factors starting from 3 */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            /* p is a prime factor, add p^4 */
            seq_t p_pow4 = (seq_t)p * (seq_t)p * (seq_t)p * (seq_t)p;
            result += p_pow4;
            
            /* Remove all factors of p */
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    /* If m > 1, then m is a prime factor */
    if (m > 1) {
        seq_t m_pow4 = (seq_t)m * (seq_t)m * (seq_t)m * (seq_t)m;
        result += m_pow4;
    }
    
    return result;
}
