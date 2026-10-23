/*
 * A135487  a(n) = (n^2+5*n+5)*(2*n+2)!/(n+4)!.
 * https://oeis.org/A135487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+5*n+5)*(2*n+2)!/(n+4)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    
    /* (n^2 + 5*n + 5) */
    seq_t base = (seq_t)n * (seq_t)n + (seq_t)5 * (seq_t)n + (seq_t)5;
    
    /* product_{k=n+5}^{2n+2} k  (empty for n=2) */
    seq_t prod = 1;
    for (idx_t k = n + 5; k <= 2 * n + 2; ++k)
        prod *= (seq_t)k;
    
    return base * prod;
}
