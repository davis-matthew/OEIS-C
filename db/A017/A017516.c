/*
 * A017516  a(n) = (11*n + 10)^8.
 * https://oeis.org/A017516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert 11*n to seq_t to keep the multiplication precise */
    seq_t base = (seq_t)11 * n + (seq_t)10;  /* base = 11*n + 10 */

    /* Compute base^8 via repeated squaring */
    seq_t result = base * base;   /* base^2 */
    result = result * result;     /* base^4 */
    result = result * result;     /* base^8 */

    return result;
}
