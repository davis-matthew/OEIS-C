/*
 * A002472  Number of pairs x,y such that y-x=2, (x,n)=1, (y,n)=1 and 1 <= x
 *          <= n.
 * https://oeis.org/A002472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   Multiplicative with a(p^e) = p^(e-1) if p = 2; (p-2)*p^(e-1) if
 *            p > 2
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    seq_t result = 1;
    seq_t temp = n;
    
    // Handle factor of 2
    if (temp % 2 == 0) {
        seq_t exp = 0;
        while (temp % 2 == 0) {
            exp++;
            temp /= 2;
        }
        // For p=2: a(2^e) = 2^(e-1)
        seq_t contribution = 1;
        for (seq_t i = 1; i < exp; i++) {
            contribution *= 2;
        }
        result *= contribution;
    }
    
    // Handle odd prime factors
    for (seq_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            // For p > 2: a(p^e) = (p-2) * p^(e-1)
            seq_t contribution = (p - 2);
            for (seq_t i = 1; i < exp; i++) {
                contribution *= p;
            }
            result *= contribution;
        }
    }
    
    // Handle remaining prime factor (if any)
    if (temp > 1) {
        result *= (temp - 2);
    }
    
    return result;
}
