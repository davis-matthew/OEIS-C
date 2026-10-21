/*
 * A230382  Number of ascending runs of length n in the permutations of [2n].
 * https://oeis.org/A230382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+n+1)*(2*n)!/((n+2)*n!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* (n^2 + n + 1) */
    seq_t pref = ((seq_t)n * (seq_t)n) + (seq_t)n + 1;

    /* Denominator n+2 */
    seq_t denom = (seq_t)(n + 2);

    /* Start with the prefactor */
    seq_t result = pref;

    /* Multiply by (n+1)(n+2)...(2n) and divide by denom afterwards.
       For values of n where result fits into seq_t, intermediate
       multiplication will not overflow before the final division. */
    for (idx_t k = n + 1; k <= 2 * n; ++k)
        result *= (seq_t)k;

    /* Final division (exact integer division) */
    result /= denom;

    return result;
}
