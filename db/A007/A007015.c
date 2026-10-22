/*
 * A007015  a(n) = smallest k such that phi(n+k) = phi(k).
 * https://oeis.org/A007015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = smallest k such that phi(n+k) = phi(k). Search method
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t phi(seq_t n) {
    if (n == 1) return 1;
    
    seq_t result = n;
    seq_t temp = n;
    
    if (temp % 2 == 0) {
        result -= result / 2;
        while (temp % 2 == 0)
            temp /= 2;
    }
    
    for (seq_t i = 3; temp / i >= i; i += 2) {
        if (temp % i == 0) {
            result -= result / i;
            while (temp % i == 0)
                temp /= i;
        }
    }
    
    if (temp > 1)
        result -= result / temp;
    
    return result;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t k = 1;
    
    while (phi(n_seq + k) != phi(k))
        k++;
    
    return k;
}
