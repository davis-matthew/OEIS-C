/*
 * A158543  a(n) = 144*n^2 - 12.
 * https://oeis.org/A158543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 - 12
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Indexing starts at 1; behaviour for n < 1 is undefined but
       we return 0 for safety. */
    if (n < 1) return 0;

    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;   /* n^2 */
    res *= 144;            /* 144 * n^2 */
    res -= 12;             /* - 12 */
    return res;
}
