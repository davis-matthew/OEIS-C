/*
 * A065482  a(n) = round( 2^n/n ).
 * https://oeis.org/A065482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(2^n / n) (n ≥ 1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; n ≥ 1 */

    const seq_t N = (seq_t)n;           /* cast for mixed comparisons */
    seq_t quotient  = 0;
    seq_t remainder = 0;

    /* Long division of the binary number 2^n by n */
    for (idx_t i = n; ; --i) {
        seq_t bit = (i == n ? 1 : 0);   /* only the MSB is 1 */
        remainder = (remainder << 1) | bit;

        if (remainder >= N) {
            remainder -= N;
            quotient = (quotient << 1) | 1;
        } else {
            quotient = quotient << 1;
        }

        if (i == 0) break;              /* finished all bits */
    }

    /* Rounding: add 1 if 2*remainder ≥ n */
    if (remainder * 2 >= N) {
        quotient += 1;
    }

    return quotient;
}
