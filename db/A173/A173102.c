/*
 * A173102  Number of partitions x + y = z with {x,y,z} in {1,2,3,..,3*n} and
 *          z > y >= x.
 * https://oeis.org/A173102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*n/4 if n is even, (9*n*n-1)/4 if n is odd
 * Range:     u8 n <= 5, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n >= 1; behaviour for n == 0 is undefined (return 0) */
    if (n == 0) return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn * (seq_t)9;

    /* test parity of n */
    if ((n & (idx_t)1) == 0)          /* n even */
        return t / (seq_t)4;
    else                               /* n odd */
        return (t - (seq_t)1) / (seq_t)4;
}
