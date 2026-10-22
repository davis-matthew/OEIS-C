/*
 * A001705  Generalized Stirling numbers, [n+2,3]_2: a(n) = n! *
 *          Sum_{k=0..n-1} (k+1)/(n-k).
 * https://oeis.org/A001705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*(n+1) + n!, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factorial = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        factorial = factorial * (seq_t)i;
        result = result * ((seq_t)i + 1) + factorial;
    }
    
    return result;
}
