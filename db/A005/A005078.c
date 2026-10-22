/*
 * A005078  Sum of primes = 1 mod 4 dividing n.
 * https://oeis.org/A005078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p if p = 1 (mod 4), 0 otherwise
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 4 == 1) {
                result += (seq_t)p;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    if (m > 1 && m % 4 == 1) {
        result += (seq_t)m;
    }
    
    return result;
}
