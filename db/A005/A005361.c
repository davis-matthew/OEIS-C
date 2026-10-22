/*
 * A005361  Product of exponents of prime factorization of n.
 * https://oeis.org/A005361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Product of exponents in prime factorization of n
 * Range:     u8 n <= 9648, u16 n <= 9648, u32 n <= 9648, u64 n <= 9648,
 *            u128 n <= 9648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            result *= (seq_t)exp;
        }
    }
    
    return result;
}
