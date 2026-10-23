/*
 * A017010  a(n) = (7*n+2)^6.
 * https://oeis.org/A017010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 231, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t base = (seq_t)n;
    /* compute (7*n + 2) */
    base = 7 * base + 2;

    /* compute base^6 by repeated multiplication */
    seq_t result = base;
    for (int i = 1; i < 6; ++i)
        result *= base;

    return result;
}
