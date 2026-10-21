/*
 * A088705  First differences of A000120. One minus exponent of 2 in n.
 * https://oeis.org/A088705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(0)=0; for n>0, a(n) = 1 - v2(n) (v2(n) = exponent of the
 *            highest power of 2 dividing n) or recursively: a(2*n) = a(n)-1,
 *            a(2*n+1) = 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Count trailing zeros in binary representation of n,
       i.e. compute v2(n). */
    idx_t e = 0;
    idx_t t = n;
    while ((t & 1) == 0) {
        ++e;
        t >>= 1;
    }
    return (seq_t)1 - (seq_t)e;
}
