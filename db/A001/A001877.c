/*
 * A001877  Number of divisors of n of the form 5k+2; a(0) = 0.
 * https://oeis.org/A001877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n of the form 5k+2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t count = 0;
    
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            if (i % 5 == 2) count++;
            idx_t j = n / i;
            if (i != j && j % 5 == 2) count++;
        }
    }
    
    return count;
}
