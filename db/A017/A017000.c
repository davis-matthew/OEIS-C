/*
 * A017000  a(n) = (7*n + 1)^8.
 * https://oeis.org/A017000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 36, u128 n <= 9362,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 7*n + 1 with seq_t arithmetic first */
    seq_t base = ((seq_t)7 * (seq_t)n) + (seq_t)1;

    /* Exponentiation by squaring: base^8 = ((base^2)^2)^2 */
    seq_t b2 = base * base;        /* base^2   */
    seq_t b4 = b2 * b2;            /* base^4   */
    seq_t b8 = b4 * b4;            /* base^8   */

    return b8;
}
