/*
 * A003967  Inverse Möbius transform of A003958.
 * https://oeis.org/A003967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   Multiplicative with a(p^e) = e+1 if p = 2; ((p-1)^(e+1)-1)/(p-2)
 *            if p > 2
 * Range:     u8 n <= 256, u16 n <= 8250, u32 n <= 8250, u64 n <= 8250,
 *            u128 n <= 8250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Factor out powers of 2
    idx_t e = 0;
    while (temp % 2 == 0) {
        e++;
        temp /= 2;
    }
    if (e > 0) {
        result *= (seq_t)(e + 1);
    }
    
    // Check odd primes
    for (idx_t p = 3; p * p <= temp; p += 2) {
        e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            // Compute sum_{i=0}^{e} (p-1)^i (geometric series for ((p-1)^(e+1)-1)/(p-2))
            seq_t term = 1;
            seq_t p_minus_1 = (seq_t)(p - 1);
            seq_t sum = 1;
            for (idx_t i = 1; i <= e; i++) {
                term *= p_minus_1;
                sum += term;
            }
            result *= sum;
        }
    }
    
    // If temp > 1, it's a prime factor with exponent 1
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        result *= p;
    }
    
    return result;
}
