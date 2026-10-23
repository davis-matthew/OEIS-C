/*
 * A128969  a(n) = (n^3 - n)*9^n.
 * https://oeis.org/A128969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n < 2) return (seq_t)0;

    /* compute 9^n */
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow9 *= (seq_t)9;

    /* compute n*(n-1)*(n+1)  ==  n^3 - n  */
    seq_t factor = (seq_t)n;
    factor *= (seq_t)(n - 1);
    factor *= (seq_t)(n + 1);

    /* multiply the two factors, truncating to seq_t */
    seq_t result = factor;
    result *= pow9;
    return result;
}
