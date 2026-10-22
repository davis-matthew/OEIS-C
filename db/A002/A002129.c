/*
 * A002129  Generalized sum of divisors function: excess of sum of odd
 *          divisors of n over sum of even divisors of n.
 * https://oeis.org/A002129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = 3-2^(e+1) if p = 2;
 *            (p^(e+1)-1)/(p-1) if p > 2
 * Range:     i8 n <= 71, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t m = n;
    
    // Factor out powers of 2
    idx_t e = 0;
    while (m % 2 == 0) {
        e++;
        m /= 2;
    }
    if (e > 0) {
        seq_t pow2 = 1;
        for (idx_t i = 0; i <= e; i++) {
            pow2 *= 2;
        }
        result *= 3 - pow2;
    }
    
    // Factor out odd primes
    for (idx_t p = 3; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) {
            e++;
            m /= p;
        }
        if (e > 0) {
            seq_t contrib = 1;
            seq_t p_pow = 1;
            for (idx_t i = 0; i < e; i++) {
                p_pow *= (seq_t)p;
                contrib += p_pow;
            }
            result *= contrib;
        }
    }
    
    // If m > 1, then m is a prime
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    
    return result;
}
