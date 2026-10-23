/*
 * A017238  a(n) = (9*n + 6)^6.
 * https://oeis.org/A017238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 179, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value (9*n + 6). */
    seq_t base = (seq_t)9 * (seq_t)n + (seq_t)6;
    /* Raise it to the 6th power by repeated multiplication. */
    seq_t res = base;
    for (int i = 1; i < 6; ++i)
        res *= base;
    return res;
}
