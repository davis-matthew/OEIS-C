/*
 * A001701  Generalized Stirling numbers, [n+2,n]_2.
 * https://oeis.org/A001701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(3n^2 + 17n + 26)/24, n > 1; a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 241, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t nn = (seq_t)n;
    seq_t nm1 = nn - 1;
    
    /* Compute 3n^2 + 17n + 26 */
    seq_t cubic = 3*nn*nn + 17*nn + 26;
    
    /* Compute n*(n-1)/2 by dividing the even factor by 2 */
    seq_t half_prod = (nn % 2 == 0) ? (nn / 2) * nm1 : nn * (nm1 / 2);
    
    /* Return (n*(n-1)/2) * (3n^2 + 17n + 26) / 12 */
    return half_prod * cubic / 12;
}
