/*
 * A129833  a(n) = Sum_{k = 0..n } binomial(n + 1, k + 1)*binomial(n, k)*k!.
 * https://oeis.org/A129833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n } binomial(n + 1, k + 1)*binomial(n, k)*k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = (seq_t)(n + 1);   /* term for k = 0 */
    seq_t sum  = term;             /* a(n) will accumulate the sum */
    for (idx_t k = 0; k < n; ++k)  /* iterate k = 0 .. n‑1 */
    {
        seq_t mult = (seq_t)(n - k);            /* n - k  (small integer) */
        term = term * mult * mult;             /* multiply first */
        term = term / (seq_t)(k + 2);          /* divide by k+2 */
        sum += term;                           /* accumulate */
    }
    return sum;
}
