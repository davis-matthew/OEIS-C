/*
 * A006473  a(n) = binomial(n,2)!/n!.
 * https://oeis.org/A006473
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,2)!/n!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Compute m = binomial(n,2) = n(n-1)/2
    idx_t m = n * (n - 1) / 2;
    
    // Compute a(n) = m! / n! = (n+1) * (n+2) * ... * m
    seq_t result = 1;
    for (idx_t i = n + 1; i <= m; i++) {
        result *= (seq_t)i;
    }
    
    return result;
}
