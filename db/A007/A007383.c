/*
 * A007383  Number of strict first-order maximal independent sets in path
 *          graph.
 * https://oeis.org/A007383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000931(n+5) - b(n), where A000931(k) = A000931(k-2) +
 *            A000931(k-3) with A000931(0)=0, A000931(1)=0, A000931(2)=1, and
 *            b(n)=1 if n is odd, b(n)=n/2+1 if n is even
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n + 5;
    
    // Compute A000931(m) using recurrence
    seq_t a0 = 0, a1 = 0, a2 = 1;
    
    for (idx_t k = 3; k <= m; k++) {
        seq_t a3 = a0 + a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    
    seq_t a931 = a2;
    
    // Compute b(n): 1 if n is odd, n/2+1 if n is even
    seq_t b_n = (n % 2 == 1) ? (seq_t)1 : (((seq_t)n) / 2 + (seq_t)1);
    
    return a931 - b_n;
}
