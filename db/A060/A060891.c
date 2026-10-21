/*
 * A060891  a(n) = n^6 - n^3 + 1.
 * https://oeis.org/A060891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n^3 + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote n to seq_t first */
    seq_t nn = (seq_t)n;

    /* Compute n^2, n^3, n^6 */
    seq_t n_sq = nn * nn;              /* n^2 */
    seq_t n_cu = n_sq * nn;            /* n^3 */
    seq_t n6   = n_cu * n_cu;          /* n^6 */

    /* a(n) = n^6 - n^3 + 1 */
    return n6 - n_cu + (seq_t)1;
}
