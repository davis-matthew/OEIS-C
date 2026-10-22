/*
 * A001065  Sum of proper divisors (or aliquot parts) of n: sum of divisors of
 *          n that are less than n.
 * https://oeis.org/A001065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = sigma(n) - n
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) {
                sum += (seq_t)i;
            }
            
            idx_t complement = n / i;
            if (complement < n && complement != i) {
                sum += (seq_t)complement;
            }
        }
    }
    
    return sum;
}
