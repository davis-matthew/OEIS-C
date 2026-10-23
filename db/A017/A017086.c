/*
 * A017086  a(n) = (8*n + 1)^10.
 * https://oeis.org/A017086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 10, u128 n <= 891,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* convert index to seq_t */
    x = x * 8 + 1;               /* compute 8*n + 1 */
    seq_t result = 1;            /* accumulate power */
    for (int i = 0; i < 10; ++i)
        result = result * x;     /* result *= x  (10 times) */
    return result;
}
