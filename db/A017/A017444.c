/*
 * A017444  a(n) = (11*n + 4)^8.
 * https://oeis.org/A017444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 11*n + 4 */
    seq_t base = (seq_t)11 * n + (seq_t)4;

    /* compute base^8 efficiently: (base^2)^4 = base^8 */
    seq_t sq  = base * base;   /* base^2  */
    seq_t q4  = sq * sq;       /* base^4  */
    seq_t result = q4 * q4;    /* base^8  */

    return result;
}
