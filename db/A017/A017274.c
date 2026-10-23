/*
 * A017274  a(n) = (10*n)^6.
 * https://oeis.org/A017274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 162, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (10*n) first */
    seq_t base = (seq_t)n * (seq_t)10;
    seq_t result = 1;
    /* multiply base six times to obtain base^6 */
    idx_t i;
    for (i = 0; i < 6; ++i)
        result *= base;
    return result;
}
