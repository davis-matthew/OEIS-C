/*
 * A001842  Number of divisors of n of the form 4*k+3.
 * https://oeis.org/A001842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count divisors of n where d ≡ 3 (mod 4)
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
            if (i % 4 == 3) count++;
            
            idx_t j = n / i;
            if (i != j && j % 4 == 3) count++;
        }
    }
    
    return count;
}
