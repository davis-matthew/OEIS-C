/*
 * A008953  a(n) is the leading digit of the n-th triangular number,
 *          n*(n+1)/2.
 * https://oeis.org/A008953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading digit of triangular number n*(n+1)/2
 * Range:     u8 n <= 23, u16 n <= 416, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t  = tn * (tn + 1) / 2;
    while (t >= 10)
        t /= 10;
    return t;
}
