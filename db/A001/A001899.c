/*
 * A001899  Number of divisors of n of the form 5k+4; a(0) = 0.
 * https://oeis.org/A001899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of divisors of n of the form 5k+4; count divisors d where
 *            d ≡ 4 (mod 5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t count = 0;
    
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i % 5 == 4) {
                count++;
            }
            if (i * i != n) {
                idx_t other = n / i;
                if (other % 5 == 4) {
                    count++;
                }
            }
        }
    }
    
    return count;
}
