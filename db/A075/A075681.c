/*
 * A075681  a(n) = (n-1)*(n-2)^3 - A003878(n-3), with a(1) = a(2) = 0 and a(3)
 *          = 2.
 * https://oeis.org/A075681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n^3 + 7*n^2 - 46*n + 50) for n>3; a(1)=0, a(2)=0,
 *            a(3)=2
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)2;

    seq_t tn = (seq_t)n;
    seq_t value = tn * tn * tn            /* n^3 */
               + (seq_t)7 * tn * tn        /* 7*n^2 */
               - (seq_t)46 * tn            /* -46*n */
               + (seq_t)50;                /* +50 */

    return value / 2;     /* the result is always even */
}
