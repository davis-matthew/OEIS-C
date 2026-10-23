/*
 * A017501  a(n) = (11*n + 9)^5.
 * https://oeis.org/A017501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 647, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 11*n + 9 */
    seq_t base = (seq_t)n * 11 + 9;
    /* raise base to the 5th power */
    seq_t res = 1;
    for (idx_t i = 0; i < 5; ++i) {
        res *= base;
    }
    return res;
}
