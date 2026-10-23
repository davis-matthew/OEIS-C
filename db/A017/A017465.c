/*
 * A017465  a(n) = (11*n + 6)^5.
 * https://oeis.org/A017465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 647, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid mixing types */
    seq_t x = (seq_t)n;

    /* Compute the base (11*n + 6) */
    seq_t base = 11 * x + 6;

    /* Compute base^5 by repeated multiplication */
    seq_t result = base;
    for (idx_t i = 1; i < 5; ++i) {
        result = result * base;
    }

    return result;
}
