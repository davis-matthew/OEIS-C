/*
 * A131476  a(n) = floor(n^3/3).
 * https://oeis.org/A131476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^3 / 3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                     /* cast index to seq_t */
    seq_t cube = nn * nn * nn;        /* compute n^3 */
    return cube / 3;                  /* floor division by 3 */
}
