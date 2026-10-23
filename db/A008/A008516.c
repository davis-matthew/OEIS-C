/*
 * A008516  6-dimensional centered cube numbers.
 * https://oeis.org/A008516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6*n + 15*n^2 + 20*n^3 + 15*n^4 + 6*n^5 + 2*n^6
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 1447,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;        /* n^2 */
    seq_t n_cu = n_sq * nn;      /* n^3 */
    seq_t n4   = n_cu * nn;      /* n^4 */
    seq_t n5   = n4 * nn;        /* n^5 */
    seq_t n6   = n5 * nn;        /* n^6 */

    seq_t result = 1;
    result += 6 * nn;
    result += 15 * n_sq;
    result += 20 * n_cu;
    result += 15 * n4;
    result += 6 * n5;
    result += 2 * n6;

    return result;
}
