/*
 * A003557  n divided by largest squarefree divisor of n; if n = Product
 *          p(k)^e(k) then a(n) = Product p(k)^(e(k)-1), with a(1) = 1.
 * https://oeis.org/A003557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = Product p(k)^(e(k)-1) where n = Product p(k)^e(k);
 *            multiplicative with a(p^e) = p^(e-1)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t temp = (seq_t)n;
    
    for (seq_t p = 2; p * p <= temp; p++) {
        seq_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        for (seq_t i = 1; i < exp; i++) {
            result *= p;
        }
    }
    
    return result;
}
