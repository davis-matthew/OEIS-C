/*
 * A005879  Theta series of D_4 lattice with respect to deep hole.
 * https://oeis.org/A005879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 8 * sigma(2*n + 1), where sigma(m) is the sum of divisors
 *            of m
 * Range:     u8 n <= 9, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = 2 * (seq_t)n + 1;
    seq_t sum = 0;
    seq_t i;
    
    for (i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            sum += i;
            seq_t quotient = m / i;
            if (i != quotient) {
                sum += quotient;
            }
        }
    }
    
    return 8 * sum;
}
