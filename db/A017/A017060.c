/*
 * A017060  a(n) = (7*n + 6)^8.
 * https://oeis.org/A017060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 35, u128 n <= 9361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 7*n + 6 */
    seq_t base = (seq_t)n * (seq_t)7 + (seq_t)6;

    /* Compute successive powers by squaring */
    seq_t sq  = base * base;     /* (7n + 6)^2  */
    seq_t sq4 = sq * sq;         /* (7n + 6)^4  */
    seq_t sq8 = sq4 * sq4;       /* (7n + 6)^8  */

    return sq8;
}
