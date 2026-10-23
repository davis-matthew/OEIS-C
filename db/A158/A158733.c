/*
 * A158733  a(n) = 1225*n^2 + 35.
 * https://oeis.org/A158733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1225*n^2 + 35
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 1872, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the wider sequence type first. */
    seq_t ns = (seq_t)n;

    /* Compute n^2, then multiply by 1225, and finally add 35. */
    seq_t sq = ns * ns;        /* n^2 */
    seq_t term = 1225 * sq;    /* 1225*n^2 */
    seq_t result = term + 35;  /* 1225*n^2 + 35 */

    return result;
}
