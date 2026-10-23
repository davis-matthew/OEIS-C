/*
 * A017085  a(n) = (8*n + 1)^9.
 * https://oeis.org/A017085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 17, u128 n <= 2388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base (8*n + 1) in the target sequence type */
    seq_t base = (seq_t)n * 8 + 1;

    /* fast exponentiation: base^9 = ((base^2)^4)*base  */
    seq_t t2  = base * base;        /* base^2   */
    seq_t t4  = t2   * t2;          /* base^4   */
    seq_t t8  = t4   * t4;          /* base^8   */
    seq_t result = t8 * base;       /* base^9   */

    return result;
}
