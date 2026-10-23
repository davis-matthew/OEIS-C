/*
 * A103885  a(n) = [x^(2*n)] ((1 + x)/(1 - x))^n.
 * https://oeis.org/A103885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2*n)] ((1 + x)/(1 - x))^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2, a(n) = sum_{i=1..n} 2^i * C(n,i) * C(2n-1,i-1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum  = 0;
    seq_t bin1 = 1;           /* C(n,0)   */
    seq_t bin2 = 1;           /* C(2n-1,0) */
    seq_t pow2 = 1;           /* 2^0      */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;                           /* pow2 = 2^i */

        /* Update bin1 to C(n,i) */
        bin1 = bin1 * (seq_t)(n - i + 1) / (seq_t)i;

        /* Add contribution for this i */
        sum += bin1 * bin2 * pow2;

        /* Prepare bin2 for next i+1: C(2n-1,i) */
        if (i < n) {
            bin2 = bin2 * (seq_t)(2 * n - i) / (seq_t)i;
        }
    }

    return sum;
}
