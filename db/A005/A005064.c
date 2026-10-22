/*
 * A005064  Sum of cubes of primes dividing n.
 * https://oeis.org/A005064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of cubes of distinct prime divisors of n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    
    // Check for factor 2
    if (m % 2 == 0) {
        result += 8;
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Check for odd factors
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t p_seq = (seq_t)p;
            result += p_seq * p_seq * p_seq;
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1, it's a prime factor
    if (m > 1) {
        seq_t m_seq = (seq_t)m;
        result += m_seq * m_seq * m_seq;
    }
    
    return result;
}
