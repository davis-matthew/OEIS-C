/*
 * A156774  a(n) = 6561*n^2 - 3564*n + 485.
 * https://oeis.org/A156774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6561*n^2 - 3564*n + 485
 * Range:     u8 none, u16 n <= 3, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;          /* n^2 */
    val *= 6561;                  /* 6561*n^2 */
    val -= 3564 * nn;             /* subtract 3564*n */
    val += 485;                   /* add constant 485 */
    return val;
}
