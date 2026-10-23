/*
 * A017472  a(n) = (11*n + 6)^12.
 * https://oeis.org/A017472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (11 * n + 6) in the appropriate type */
    seq_t base = (seq_t)(11 * n + 6);

    /* Raise the base to the 12th power */
    seq_t result = 1;
    for (int i = 0; i < 12; ++i) {
        result *= base;
    }

    return result;
}
