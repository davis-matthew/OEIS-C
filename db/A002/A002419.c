/*
 * A002419  4-dimensional figurate numbers: a(n) = (6*n-2)*binomial(n+2,3)/4.
 * https://oeis.org/A002419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*binomial(n+2,3)/2
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 317, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    
    // Compute binomial(n+2, 3) = n*(n+1)*(n+2)/6
    // Divide by 6 carefully to avoid overflow: one of {n,n+1,n+2} is divisible by 2, one by 3
    seq_t binom;
    if (m % 6 == 0) {
        binom = (m / 6) * (m + 1) * (m + 2);
    } else if (m % 3 == 0) {
        // m divisible by 3 but not 6, so m is odd and m+1 is even
        binom = (m / 3) * ((m + 1) / 2) * (m + 2);
    } else if (m % 2 == 0) {
        // m is even but not divisible by 3
        if ((m + 2) % 3 == 0) {
            binom = (m / 2) * (m + 1) * ((m + 2) / 3);
        } else {
            binom = (m / 2) * ((m + 1) / 3) * (m + 2);
        }
    } else {
        // m is odd and not divisible by 3, so m+1 is even
        if ((m + 1) % 3 == 0) {
            binom = m * ((m + 1) / 6) * (m + 2);
        } else {
            binom = m * ((m + 1) / 2) * ((m + 2) / 3);
        }
    }
    
    // Compute (3*n - 1) * binomial(n+2, 3) / 2
    seq_t coeff = 3 * m - 1;
    seq_t result = (coeff * binom) / 2;
    return result;
}
