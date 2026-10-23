/*
 * A011915  a(n) = floor(n*(n-1)*(n-2)*(n-3)/5).
 * https://oeis.org/A011915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)*(n-3) / 5 )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 267, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product contains a factor 0. */
    if (n < 4)
        return (seq_t)0;

    seq_t res = (seq_t)n;
    res *= (seq_t)(n - 1);
    res *= (seq_t)(n - 2);
    res *= (seq_t)(n - 3);
    res /= (seq_t)5;      /* integer division truncates toward zero, which is floor for positives */

    return res;
}
