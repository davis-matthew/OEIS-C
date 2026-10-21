/*
 * A075412  Squares of A002277.
 * https://oeis.org/A075412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10^n - 1)/3)^2 The decimal form of this square is: (n-1
 *            times '1') followed by '0', then (n-1 times '8') followed by '9'
 *            for n >= 1; a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t res = 0;
    /* (n-1) leading '1's */
    for (idx_t i = 0; i + 1 < n; ++i)
        res = res * (seq_t)10 + (seq_t)1;

    /* the '0' digit */
    res = res * (seq_t)10 + (seq_t)0;

    /* (n-1) '8's */
    for (idx_t i = 0; i + 1 < n; ++i)
        res = res * (seq_t)10 + (seq_t)8;

    /* trailing '9' */
    res = res * (seq_t)10 + (seq_t)9;

    return res;
}
