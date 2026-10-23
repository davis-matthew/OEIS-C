/*
 * A017020  a(n) = (7*n + 3)^4.
 * https://oeis.org/A017020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 36, u64 n <= 9361,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)7 * n + 3;    /* compute 7*n + 3 */
    seq_t x2 = x * x;              /* (7*n + 3)^2 */
    return x2 * x2;                /* square again -> (7*n + 3)^4 */
}
