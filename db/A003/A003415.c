/*
 * A003415  a(n) = n' = arithmetic derivative of n: a(0) = a(1) = 0, a(prime)
 *          = 1, a(m*n) = m*a(n) + n*a(m).
 * https://oeis.org/A003415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, hear, look
 * Formula:   a(n) = Sum_{p|n} (n/p) * v_p(n), where v_p(n) is the exponent of
 *            prime p
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1)
        return 0;
    
    seq_t result = 0;
    seq_t original_n = (seq_t)n;
    seq_t remaining = original_n;
    
    // Trial division to find prime factors and their exponents
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (original_n / p) * (seq_t)exponent;
        }
    }
    
    // If remaining > 1, it's a prime factor with exponent 1
    if (remaining > 1) {
        result += original_n / remaining;
    }
    
    return result;
}
