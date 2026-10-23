/*
 * A016998  a(n) = (7*n + 1)^6.
 * https://oeis.org/A016998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 232, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 7*n + 1 */
    seq_t b = (seq_t)7 * (seq_t)n + 1;
    /* raise to the 6th power by successive multiplication */
    seq_t res = b * b * b * b * b * b;
    return res;
}
