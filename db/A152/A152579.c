/*
 * A152579  a(n) = (10*n+3)*(10*n+17).
 * https://oeis.org/A152579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n+3)*(10*n+17)
 * Range:     u8 n <= 0, u16 n <= 24, u32 n <= 6552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* cast index to seq_t */
    seq_t x  = 10U * tn + 3U;     /* 10*n + 3 */
    seq_t y  = 10U * tn + 17U;    /* 10*n + 17 */
    return x * y;                 /* product */
}
