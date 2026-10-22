/*
 * A005094  Number of distinct primes of the form 4k+1 dividing n minus number
 *          of distinct primes of the form 4k+3 dividing n.
 * https://oeis.org/A005094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   Additive with a(p^e) = 0 if p = 2, 1 if p == 1 (mod 4), -1 if p
 *            == 3 (mod 4)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    uint64_t m = n;
    
    // Remove factor of 2 (contributes 0)
    if (m % 2 == 0) {
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    
    // Try odd divisors starting from 3
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            // p is a prime factor
            if (p % 4 == 1) {
                result += 1;
            } else {
                result -= 1;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1, then m is a prime factor
    if (m > 1) {
        if (m % 4 == 1) {
            result += 1;
        } else {
            result -= 1;
        }
    }
    
    return result;
}
