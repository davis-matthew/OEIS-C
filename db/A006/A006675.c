/*
 * A006675  Number of paths through an array.
 * https://oeis.org/A006675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*n! * (H(n) - 1), H(n) = Sum_{k=1..n} 1/k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    
    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; k++) {
        sum += factorial / (seq_t)k;
    }
    
    return (seq_t)n * (sum - factorial);
}
