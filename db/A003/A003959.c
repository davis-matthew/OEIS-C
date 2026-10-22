/*
 * A003959  If n = Product p(k)^e(k) then a(n) = Product (p(k)+1)^e(k), a(1) =
 *          1.
 * https://oeis.org/A003959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   If n = Product p^e then a(n) = Product (p+1)^e; a(1) = 1
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 9655, u64 n <= 9655,
 *            u128 n <= 9655, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        idx_t exponent = 0;
        while (temp % p == 0) {
            exponent++;
            temp /= p;
        }
        seq_t factor = (seq_t)(p + 1);
        for (idx_t i = 0; i < exponent; i++) {
            result *= factor;
        }
    }
    
    if (temp > 1) {
        result *= (seq_t)(temp + 1);
    }
    
    return result;
}
