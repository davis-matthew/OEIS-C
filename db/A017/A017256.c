/*
 * A017256  a(n) = (9*n + 7)^12.
 * https://oeis.org/A017256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 7)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 9*n + 7 */
    seq_t base = ((seq_t)9 * (seq_t)n) + 7;

    /* compute base^12 via simple multiplication */
    seq_t result = 1;
    for (int i = 0; i < 12; ++i) {
        result *= base;
    }

    return result;
}
