/*
 * A005728  Number of fractions in Farey series of order n.
 * https://oeis.org/A005728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + Sum_{i=1..n} phi(i)
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 8963, u64 n <= 8963,
 *            u128 n <= 8963, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t phi(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = (seq_t)n;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            result = result / (seq_t)p * ((seq_t)p - 1);
        }
    }
    
    if (temp > 1) {
        result = result / (seq_t)temp * ((seq_t)temp - 1);
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 1;
    for (idx_t i = 1; i <= n; i++) {
        sum += phi(i);
    }
    return sum;
}
