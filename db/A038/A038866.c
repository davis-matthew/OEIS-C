/*
 * A038866  a(n) = (n+4)^3 - n^3.
 * https://oeis.org/A038866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)^3 - n^3 = 12*n^2 + 48*n + 64
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once */
    seq_t nn = (seq_t)n;

    /* compute 12 * n^2 */
    seq_t t = nn * nn;          /* n^2 */
    t = 12 * t;                 /* 12*n^2 */

    /* add 48*n */
    t += 48 * nn;               /* +48*n */

    /* add constant 64 */
    t += 64;

    return t;
}
