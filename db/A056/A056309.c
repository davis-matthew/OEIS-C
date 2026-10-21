/*
 * A056309  Number of reversible strings with n beads using exactly two
 *          different colors.
 * https://oeis.org/A056309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? 2^(n/2-1) + 2^(n-1) - 2 : 2^((n-1)/2) +
 *            2^(n-1) - 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* problem offset starts at 1 */
    idx_t exp2 = n - 1;                       /* common power of two */
    idx_t exp1;                               /* the smaller exponent */

    if ((n & 1) == 0) /* n is even */
        exp1 = (n >> 1) - 1;                  /* n/2 - 1 */
    else               /* n is odd  */
        exp1 = (n - 1) >> 1;                  /* (n-1)/2 */

    seq_t term = ((seq_t)1 << exp1) + ((seq_t)1 << exp2);
    term -= (seq_t)2;
    return term;
}
