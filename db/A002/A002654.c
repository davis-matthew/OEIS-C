/*
 * A002654  Number of ways of writing n as a sum of at most two nonzero
 *          squares, where order matters; also (number of divisors of n of
 *          form 4m+1) - (number of divisors of form 4m+3).
 * https://oeis.org/A002654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, nice, mult, changed
 * Formula:   a(n) = product over prime powers p^e in factorization of n: - 1
 *            if p = 2 - (e+1) if p ≡ 1 (mod 4) - (e+1) mod 2 if p ≡ 3 (mod 4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    
    // Handle factor of 2: contributes 1
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    // Check odd factors
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exponent = 0;
            while (temp % p == 0) {
                exponent++;
                temp /= p;
            }
            
            if (p % 4 == 1) {
                // p ≡ 1 (mod 4): contribute (exponent + 1)
                result *= (seq_t)(exponent + 1);
            } else {
                // p ≡ 3 (mod 4): contribute (exponent + 1) mod 2
                // which is 0 if exponent is odd, 1 if exponent is even
                if (exponent % 2 == 1) {
                    return 0;
                }
            }
        }
    }
    
    // Handle the remaining factor (if any, it's a prime > sqrt(original temp))
    if (temp > 1) {
        if (temp % 4 == 1) {
            // p ≡ 1 (mod 4): exponent = 1, contribute 2
            result *= 2;
        } else {
            // p ≡ 3 (mod 4): exponent = 1 (odd), contribute 0
            return 0;
        }
    }
    
    return result;
}
