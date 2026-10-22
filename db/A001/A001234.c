/*
 * A001234  Unsigned Stirling numbers of the first kind s(n,7).
 * https://oeis.org/A001234
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = unsigned Stirling number of the first kind s(n,7)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via recurrence: c(n,k) = (n-1)*c(n-1,k) + c(n-1,k-1), c(0,0)=1 */

seq_t a(idx_t n)
{
    if (n < 7) return 0;
    
    seq_t prev[8], curr[8];
    
    prev[0] = 1;
    for (int k = 1; k <= 7; k++) {
        prev[k] = 0;
    }
    
    for (idx_t i = 1; i <= n; i++) {
        curr[0] = 0;
        for (int k = 1; k <= 7 && k <= (int)i; k++) {
            curr[k] = (seq_t)(i - 1) * prev[k] + prev[k - 1];
        }
        for (int k = (int)i + 1; k <= 7; k++) {
            curr[k] = 0;
        }
        
        for (int k = 0; k <= 7; k++) {
            prev[k] = curr[k];
        }
    }
    
    return prev[7];
}
