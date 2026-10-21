/*
 * A088336  Number of permutations in the symmetric group S_n that have even
 *          number of transpositions in their cycle decomposition.
 * https://oeis.org/A088336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + Σ_{k=0}^{⌊n/2⌋} (-1)^k * n! / k!) / 2 (derived from
 *            the EGF (exp(-x^2)+1)/(1-x)/2) This counts permutations of n
 *            with an even number of transpositions in their cycle
 *            decomposition
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! as fact */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* term = n! / k! , starts with k = 0 */
    seq_t term = fact;
    idx_t m = n / 2;

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (idx_t k = 0; k <= m; ++k)
    {
        if ((k & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;

        if (k < m)
            term /= (k + 1);   /* term_{k+1} = term_k / (k+1) */
    }

    seq_t temp = fact + sum_even;
    temp -= sum_odd;
    return temp / 2;
}
