/*
 * A007584  9-gonal (or enneagonal) pyramidal numbers: a(n) =
 *          n*(n+1)*(7*n-4)/6.
 * https://oeis.org/A007584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n-4)/6
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    
    // Compute n*(n+1)/2 (always an integer since n and n+1 are consecutive)
    seq_t half_product = nn * (nn + 1) / 2;
    
    // Multiply by (7*n-4) and divide by 3
    return half_product * (7 * nn - 4) / 3;
}
