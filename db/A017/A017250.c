/*
 * A017250  a(n) = (9*n + 7)^6.
 * https://oeis.org/A017250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 7)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 179, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;   /* cast index to sequence type */
    x = 9 * x + 7;        /* compute the base (9*n + 7) */

    /* Raise to the 6th power: r = x^6 */
    seq_t r = 1;
    for (idx_t i = 0; i < 6; ++i)
        r *= x;

    return r;
}
