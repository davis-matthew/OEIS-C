/*
 * A016908  a(n) = (5*n + 4)^12.
 * https://oeis.org/A016908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base: 5*n + 4, all in seq_t to avoid truncation */
    seq_t base = (seq_t)5 * n + (seq_t)4;
    /* Raise the base to the 12th power by repeated multiplication */
    seq_t res = 1;
    for (int i = 0; i < 12; ++i)
        res *= base;
    return res;
}
