/*
 * A007428  Moebius transform applied thrice to sequence 1,0,0,0,....
 * https://oeis.org/A007428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = (3 choose e) (-1)^e
 * Range:     i8 n <= 2309, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    static const seq_t coeff[] = {1, -3, 3, -1};
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Factor out all 2's
    idx_t e = 0;
    while (temp % 2 == 0) {
        e++;
        temp /= 2;
    }
    if (e > 0) {
        if (e >= 4) return 0;
        result *= coeff[e];
    }
    
    // Factor out odd primes
    for (idx_t p = 3; p <= temp / p; p += 2) {
        e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            if (e >= 4) return 0;
            result *= coeff[e];
        }
    }
    
    // Handle remaining prime factor if any
    if (temp > 1) {
        result *= coeff[1];
    }
    
    return result;
}
