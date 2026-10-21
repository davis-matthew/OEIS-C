/*
 * A230346  Number of permutations of [2n+6] in which the longest increasing
 *          run has length n+6.
 * https://oeis.org/A230346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+5)*(n^2+9*n+11)*(2*n+6)!/(n+8)! for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 2 the factorial ratio equals ∏_{i=n+9}^{2n+6} i. For n=1 the ratio is 1/(n+8)=1/9. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Base term (n+5)*(n^2+9*n+11) */
    seq_t term = (seq_t)(n + 5) * (((seq_t)n * n) + 9 * (seq_t)n + 11);

    if (n == 1) {
        /* ratio = 1/(n+8) = 1/9 */
        return term / (n + 8);
    }

    /* n >= 2 */
    seq_t result = term;

    /* Multiply by the product from n+9 up to 2n+6 (empty product for n=2) */
    for (idx_t i = n + 9; i <= 2 * n + 6; ++i)
        result *= (seq_t)i;

    return result;
}
