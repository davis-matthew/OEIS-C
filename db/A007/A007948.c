/*
 * A007948  Largest cubefree number dividing n.
 * https://oeis.org/A007948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of p^min(e, 2) for each prime power p^e in n's
 *            factorization
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    
    // Check for factor 2
    if (temp % 2 == 0) {
        int count = 0;
        while (temp % 2 == 0) {
            count++;
            temp /= 2;
        }
        if (count >= 2) {
            result *= 4;
        } else {
            result *= 2;
        }
    }
    
    // Check for odd factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int count = 0;
            while (temp % p == 0) {
                count++;
                temp /= p;
            }
            seq_t p_seq = (seq_t)p;
            if (count >= 2) {
                result *= p_seq * p_seq;
            } else {
                result *= p_seq;
            }
        }
    }
    
    // If temp > 1, it's a prime factor with exponent 1
    if (temp > 1) {
        result *= (seq_t)temp;
    }
    
    return result;
}
