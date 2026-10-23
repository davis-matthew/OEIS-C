/*
 * A186853  Number of 4-step knight's tours on an (n+2) X (n+2) board summed
 *          over all starting positions.
 * https://oeis.org/A186853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 for n = 1 208 for n = 2 976 for n = 3 392*n^2 - 1048*n
 *            + 496 for n > 3
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 16;
    if (n == 2) return 208;
    if (n == 3) return 976;
    /* n >= 4 */
    seq_t nn = (seq_t)n;
    /* compute 392*n^2 - 1048*n + 496 as n*(392*n - 1048) + 496 */
    seq_t val = nn * ((seq_t)392 * nn - 1048) + 496;
    return val;
}
