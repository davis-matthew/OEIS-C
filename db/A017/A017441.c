/*
 * A017441  a(n) = (11*n + 4)^5.
 * https://oeis.org/A017441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 647, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base (11*n + 4) */
    seq_t v = (seq_t)11 * n + (seq_t)4;

    /* raise v to the 5th power by repeated multiplication */
    seq_t r = v;       /* v¹  */
    r *= v;            /* v²  */
    r *= v;            /* v³  */
    r *= v;            /* v⁴  */
    r *= v;            /* v⁵  */
    return r;
}
