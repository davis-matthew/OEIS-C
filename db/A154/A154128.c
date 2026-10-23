/*
 * A154128  a(n) = 5^n*(n+4)!/n!.
 * https://oeis.org/A154128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n*(n+4)*(n+3)*(n+2)*(n+1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;           /* a(0) = 24 */

    seq_t res = (seq_t)24;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* a(i) = a(i-1) * 5 * (i+4) / i
         * Since a(i-1) is divisible by i, divide first to keep
         * intermediate values small. */
        res = res / (seq_t)i;            /* exact division by i */
        res *= (seq_t)5;                 /* multiply by 5 */
        res *= (seq_t)(i + 4);           /* multiply by (i+4) */
    }
    return res;
}
