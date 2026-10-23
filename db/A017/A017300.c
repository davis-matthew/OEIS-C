/*
 * A017300  a(n) = (10*n + 2)^8.
 * https://oeis.org/A017300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 25, u128 n <= 6553,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 10*n + 2 in seq_t */
    seq_t base = (seq_t)n;
    base *= (seq_t)10;
    base += (seq_t)2;

    /* compute base^8 using repeated squaring */
    seq_t t1 = base * base;     /* base^2   */
    seq_t t2 = t1 * t1;         /* base^4   */
    seq_t t3 = t2 * t2;         /* base^8   */

    return t3;
}
