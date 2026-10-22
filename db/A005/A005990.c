/*
 * A005990  a(n) = (n-1)*(n+1)!/6.
 * https://oeis.org/A005990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+1)!/6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    // Compute ((n-1)*n*(n+1))/6, then multiply by (n-1)!
    seq_t prod = ((seq_t)n - 1) * (seq_t)n * ((seq_t)n + 1) / 6;
    
    for (idx_t i = 2; i < n; i++) {
        prod *= (seq_t)i;
    }
    
    return prod;
}
