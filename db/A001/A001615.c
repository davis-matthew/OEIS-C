/*
 * A001615  Dedekind psi function: n * Product_{p|n, p prime} (1 + 1/p).
 * https://oeis.org/A001615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice, mult, changed
 * Formula:   Multiplicative with a(p^e) = (p+1)*p^(e-1)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            /* Contribute (p+1)*p^(exp-1) for this prime power */
            seq_t contribution = (seq_t)(p + 1);
            for (idx_t i = 1; i < exp; i++) {
                contribution *= (seq_t)p;
            }
            result *= contribution;
        }
    }
    
    if (temp > 1) {
        /* temp is a prime factor with exponent 1 */
        result *= (seq_t)(temp + 1);
    }
    
    return result;
}
