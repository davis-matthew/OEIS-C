/*
 * A224411  Number of 4 X n 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 16 a(2) = 108 For n > 2, a(n) = (1/40320) * [ n^8 + 28
 *            n^7 + 434 n^6 + 3640 n^5 + 26369 n^4 + 179452 n^3 + 759436 n^2 -
 *            122640 n - 282240 ] (This is the empirical polynomial given in
 *            the OEIS entry.)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 11, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */
    if (n == 1) return (seq_t)16;  /* a(1) */
    if (n == 2) return (seq_t)108; /* a(2) */

    /* Compute powers of n as seq_t to avoid loss of precision. */
    seq_t tn   = (seq_t)n;     /* n^1   */
    seq_t tn2  = tn * tn;      /* n^2   */
    seq_t tn3  = tn2 * tn;     /* n^3   */
    seq_t tn4  = tn2 * tn2;    /* n^4   */
    seq_t tn5  = tn4 * tn;     /* n^5   */
    seq_t tn6  = tn3 * tn3;    /* n^6   */
    seq_t tn7  = tn6 * tn;     /* n^7   */
    seq_t tn8  = tn4 * tn4;    /* n^8   */

    /* Numerator of the polynomial expression. */
    seq_t num = tn8;
    num += (seq_t)28   * tn7;
    num += (seq_t)434  * tn6;
    num += (seq_t)3640 * tn5;
    num += (seq_t)26369 * tn4;
    num += (seq_t)179452 * tn3;
    num += (seq_t)759436 * tn2;
    num -= (seq_t)122640 * tn;
    num -= (seq_t)282240;

    /* Denominator is 40320; integer division gives the exact result. */
    return num / (seq_t)40320;
}
