/*
 * A005077  Sum of 4th powers of primes = 2 mod 3 dividing n.
 * https://oeis.org/A005077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of p^4 for each prime p | n such that p ≡ 2 (mod 3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    
    // Factor out 2 (which is ≡ 2 mod 3)
    if (m % 2 == 0) {
        result += (seq_t)16;  // 2^4 = 16
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Factor out odd primes
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 3 == 2) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq * p_seq * p_seq;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // Check remaining prime factor (if any)
    if (m > 1 && m % 3 == 2) {
        seq_t p_seq = (seq_t)m;
        result += p_seq * p_seq * p_seq * p_seq;
    }
    
    return result;
}
