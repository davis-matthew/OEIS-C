/*
 * A017333  a(n) = (10*n + 5)^5.
 * https://oeis.org/A017333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t first, then compute the value */
    seq_t x = (seq_t)10 * (seq_t)n + (seq_t)5;
    /* Compute the fifth power using successive multiplication */
    seq_t res = x * x * x * x * x;
    return res;
}
