/*
 * A004011  Theta series of D_4 lattice; Fourier coefficients of Eisenstein
 *          series E_{gamma,2}.
 * https://oeis.org/A004011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(0) = 1; a(n) = 24 * A000593(n) for n > 0
 * Range:     u8 n <= 8, u16 n <= 1484, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t n_seq = (seq_t)n;
    
    // Remove factors of 2 to get the odd part
    seq_t odd_part = n_seq;
    while (odd_part % 2 == 0) {
        odd_part /= 2;
    }
    
    // Sum of all divisors of odd_part (which are all odd)
    seq_t sum = 0;
    for (seq_t i = 1; i * i <= odd_part; i += 2) {
        if (odd_part % i == 0) {
            sum += i;
            if (i != odd_part / i) {
                sum += odd_part / i;
            }
        }
    }
    
    return 24 * sum;
}
