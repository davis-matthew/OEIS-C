/*
 * A060888  a(n) = n^6 - n^5 + n^4 - n^3 + n^2 - n + 1.
 * https://oeis.org/A060888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n^5 + n^4 - n^3 + n^2 - n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;           /* n          */
    seq_t x2 = x * x;              /* n^2        */
    seq_t x3 = x2 * x;             /* n^3        */
    seq_t x4 = x2 * x2;            /* n^4        */
    seq_t x5 = x4 * x;             /* n^5        */
    seq_t x6 = x3 * x3;            /* n^6        */

    seq_t res = x6;
    res -= x5;
    res += x4;
    res -= x3;
    res += x2;
    res -= x;
    res += (seq_t)1;
    return res;
}
