/*
 * A106633  Number of ways to express n as k+l*m, with k, l, m all in the
 *          range [0..n].
 * https://oeis.org/A106633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 1 + Sum_{k=1..n} floor(n/k)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n + 1 first */
    seq_t result = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* Add the sum of floor(n/k) for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
        result += (seq_t)(n / k);

    return result;
}
