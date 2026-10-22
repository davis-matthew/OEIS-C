/*
 * A007437  Inverse Moebius transform of triangular numbers.
 * https://oeis.org/A007437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d*(d+1)/2
 * Range:     u8 n <= 19, u16 n <= 299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            seq_t d = (seq_t)i;
            seq_t tri = d * (d + 1) / 2;
            result += tri;
            
            if (i != n / i) {
                idx_t j = n / i;
                seq_t dj = (seq_t)j;
                seq_t tri_j = dj * (dj + 1) / 2;
                result += tri_j;
            }
        }
    }
    
    return result;
}
