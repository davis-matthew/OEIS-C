/*
 * A017178  a(n) = (9*n + 1)^6.
 * https://oeis.org/A017178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 9*n + 1 */
    seq_t t = (seq_t)9 * n + 1;

    /* Fast exponentiation: t^6 = t^4 * t^2 */
    seq_t t2 = t * t;          /* t^2 */
    seq_t t4 = t2 * t2;        /* t^4 */
    seq_t result = t4 * t2;    /* t^6 */

    return result;
}
