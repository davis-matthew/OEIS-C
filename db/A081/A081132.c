/*
 * A081132  a(n) = (n+1)^n*binomial(n+2,2).
 * https://oeis.org/A081132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)^n*binomial(n+2,2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = n+1 */
    seq_t base = (seq_t)(n + 1);

    /* Compute base^n  */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= base;           /* pow = (n+1)^n */

    /* binomial(n+2,2) = (n+1)*(n+2)/2  */
    seq_t bin = (base * (seq_t)(n + 2)) / 2;

    return pow * bin;          /* a(n) = base^n * binomial(n+2,2) */
}
