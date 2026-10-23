/*
 * A027659  a(n) = binomial(n+2,2) + binomial(n+3,3) + binomial(n+4,4) +
 *          binomial(n+5,5).
 * https://oeis.org/A027659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,5) - (n+2) (n ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 216, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        /* Multiply by (n+1+i) and divide by i; 
           the division is exact for binomial coefficients.   */
        res = res * ((seq_t)n + 1 + i) / i;
    }
    return res - ((seq_t)n + 2);
}
