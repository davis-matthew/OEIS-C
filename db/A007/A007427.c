/*
 * A007427  Moebius transform applied twice to sequence 1,0,0,0,....
 * https://oeis.org/A007427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   multiplicative with a(p^e) = binomial(2,e)*(-1)^e for prime p
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    
    // Factor out 2
    int exp = 0;
    while (m % 2 == 0) {
        exp++;
        m /= 2;
    }
    if (exp >= 3) return 0;
    if (exp == 1) result *= -2;
    
    // Factor out odd primes
    for (idx_t p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) {
            exp++;
            m /= p;
        }
        if (exp >= 3) return 0;
        if (exp == 1) result *= -2;
    }
    
    // If m > 1, it's a prime factor with exponent 1
    if (m > 1) {
        result *= -2;
    }
    
    return result;
}
