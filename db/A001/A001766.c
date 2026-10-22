/*
 * A001766  Index of (the image of) the modular group Gamma(n) in PSL_2(Z).
 * https://oeis.org/A001766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3/2) * Product_{p|n, p prime} (1 - 1/p^2), for n >= 3;
 *            a(1) = 1, a(2) = 6
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 6;
    
    seq_t result = (seq_t)n;
    result = result * result * result;  // n^3
    
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t p_seq = (seq_t)p;
            seq_t p2 = p_seq * p_seq;
            result = result * (p2 - 1) / p2;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1) {
        seq_t p_seq = (seq_t)temp;
        seq_t p2 = p_seq * p_seq;
        result = result * (p2 - 1) / p2;
    }
    
    result = result / 2;
    return result;
}
