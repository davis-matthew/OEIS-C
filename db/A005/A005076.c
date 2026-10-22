/*
 * A005076  Sum of cubes of primes = 2 mod 3 dividing n.
 * https://oeis.org/A005076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Additive with a(p^e) = p^3 if p ≡ 2 (mod 3), 0 otherwise
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 1636, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    
    if (temp % 2 == 0) {
        result += 8;
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 3 == 2) {
                seq_t p_seq = (seq_t)p;
                result += p_seq * p_seq * p_seq;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1) {
        if (temp % 3 == 2) {
            seq_t p_seq = (seq_t)temp;
            result += p_seq * p_seq * p_seq;
        }
    }
    
    return result;
}
