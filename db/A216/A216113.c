/*
 * A216113  The hyper-Wiener index of the meta-polyphenyl chain with n
 *          hexagons (see the Dou et al. and the Deng references).
 * https://oeis.org/A216113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n(16n^3 + 8n^2 - 5n +9)/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Compute 16n^3 + 8n^2 - 5n + 9 */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t term = 16 * n3;        /* 16n^3 */
    term += 8 * n2;              /* + 8n^2 */
    term -= 5 * nn;              /* - 5n */
    term += 9;                   /* + 9 */

    /* Compute 3n * term / 2 */
    seq_t result = 3 * nn * term;
    result /= 2;

    return result;
}
