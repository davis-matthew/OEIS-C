/*
 * A171860  Number of n-cell fixed polycubes that are proper in n-2
 *          dimensions.
 * https://oeis.org/A171860
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3)*n^(n-5)*(n-2)*(2*n^2 - 6*n + 9)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 17;

    seq_t result = 1;

    /* Compute 2^(n-3) */
    for (idx_t i = 0; i < n - 3; ++i) result *= (seq_t)2;

    /* Compute n^(n-5) */
    for (idx_t i = 0; i < n - 5; ++i) result *= (seq_t)n;

    /* Multiply by (n-2) */
    result *= (seq_t)(n - 2);

    /* Multiply by (2*n^2 - 6*n + 9) */
    seq_t term = (seq_t)2 * (seq_t)n * (seq_t)n
                 - (seq_t)6 * (seq_t)n
                 + (seq_t)9;
    result *= term;

    return result;
}
