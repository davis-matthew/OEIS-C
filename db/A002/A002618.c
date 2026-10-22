/*
 * A002618  a(n) = n*phi(n).
 * https://oeis.org/A002618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, look
 * Formula:   a(n) = n * phi(n), where phi is Euler's totient function
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t phi = (seq_t)n;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            phi = (phi / (seq_t)p) * ((seq_t)p - 1);
        }
    }
    if (temp > 1) {
        phi = (phi / (seq_t)temp) * ((seq_t)temp - 1);
    }
    
    return (seq_t)n * phi;
}
