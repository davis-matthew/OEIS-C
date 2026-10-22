/*
 * A007391  Number of strict first-order maximal independent sets in cycle
 *          graph.
 * https://oeis.org/A007391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001608(n) - b(n) where A001608 is Perrin sequence, b(1)
 *            = 0, b(odd n > 1) = n, b(even n) = 2
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute Perrin sequence A001608(n): P(0)=3, P(1)=0, P(2)=2, P(n)=P(n-2)+P(n-3)
    seq_t p0 = (seq_t)3, p1 = (seq_t)0, p2 = (seq_t)2;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t pn = p0 + p1;
        p0 = p1;
        p1 = p2;
        p2 = pn;
    }
    
    // Get P(n)
    seq_t perrin;
    if (n == 1) {
        perrin = (seq_t)0;
    } else if (n == 2) {
        perrin = (seq_t)2;
    } else {
        perrin = p2;
    }
    
    // Compute b(n): b(1)=0, b(odd n>1)=n, b(even n)=2
    seq_t bn = (n == 1) ? (seq_t)0 : (n % 2 == 1) ? (seq_t)n : (seq_t)2;
    
    return perrin - bn;
}
