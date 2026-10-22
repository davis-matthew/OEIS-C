/*
 * A001810  a(n) = n!*n*(n-1)*(n-2)/36.
 * https://oeis.org/A001810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*n*(n-1)*(n-2)/36
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; i++) {
        fact = fact * (seq_t)i;
    }
    
    seq_t binom3 = (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2) / 6;
    return fact / 6 * binom3;
}
