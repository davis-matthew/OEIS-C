/*
 * A034856  a(n) = binomial(n+1, 2) + n - 1 = n*(n+3)/2 - 1.
 * https://oeis.org/A034856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)/2 - 1
 * Range:     u8 n <= 14, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We compute the value using seq_t arithmetic.  The sequence is
       defined for n ≥ 1; behaviour for n = 0 is irrelevant, but we
       return 0 in that case to avoid an underflow on unsigned types. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;
    seq_t value = N * (N + 3);  /* N*(N+3) */
    value = value / 2;          /* divide by 2 */
    value = value - 1;          /* subtract 1 */
    return value;
}
