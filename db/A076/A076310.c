/*
 * A076310  a(n) = floor(n/10) + 4*(n mod 10).
 * https://oeis.org/A076310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/10) + 4*(n mod 10)
 * Range:     u8 n <= 333, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t and perform the divisions and
       multiplications in the seq_t domain.  The operations are
       defined by the build for all supported seq_t types. */
    seq_t q = (seq_t)n / 10;          /* floor(n/10)   */
    seq_t r = (seq_t)n % 10;          /* n mod 10      */
    return q + (seq_t)4 * r;          /* final value   */
}
