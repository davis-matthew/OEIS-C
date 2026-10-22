/*
 * A001712  Generalized Stirling numbers, [n+5,5]_3.
 * https://oeis.org/A001712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{0 <= i < j <= n+1} Product_{r not in {i,j},
 *            0<=r<=n+1} (3+r)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    
    for (idx_t i = 0; i <= n + 1; i++) {
        for (idx_t j = i + 1; j <= n + 1; j++) {
            seq_t term = 1;
            
            for (idx_t r = 0; r < i; r++) {
                term = term * (seq_t)(3 + r);
            }
            for (idx_t r = i + 1; r < j; r++) {
                term = term * (seq_t)(3 + r);
            }
            for (idx_t r = j + 1; r <= n + 1; r++) {
                term = term * (seq_t)(3 + r);
            }
            
            result = result + term;
        }
    }
    
    return result;
}
