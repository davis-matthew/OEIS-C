/*
 * A017189  a(n) = (9*n + 2)^5.
 * https://oeis.org/A017189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 9, u64 n <= 792, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (9*n + 2) once in the target type */
    seq_t base = (seq_t)9 * (seq_t)n + (seq_t)2;

    /* Raise it to the 5th power by repeated multiplication */
    seq_t res = base;      /* base^1   */
    res *= base;           /* base^2   */
    res *= base;           /* base^3   */
    res *= base;           /* base^4   */
    res *= base;           /* base^5   */

    return res;
}
