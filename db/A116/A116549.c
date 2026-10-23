/*
 * A116549  a(0) = 1. a(m + 2^n) = a(n) + a(m), for 0 <= m <= 2^n - 1.
 * https://oeis.org/A116549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0, let e=floor(log2(n)) then a(n)=a(e)+a(n-2^e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* find e = floor(log2(n)) without external utilities */
    idx_t e = 0;
    while ((n >> (e + 1)) != 0)
        ++e;

    /* split n into 2^e and the remainder m */
    idx_t m = n - ((idx_t)1 << e);

    /* recursive definition */
    return a(e) + a(m);
}
