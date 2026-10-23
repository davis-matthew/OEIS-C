/*
 * A100832  Amenable numbers: n such that there exists a multiset of integers
 *          (s(1), ..., s(n)) whose size, sum and product are all n.
 * https://oeis.org/A100832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=2*n+1 if n is even, a(n)=2*n+2 if n is odd
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t t = (seq_t)n;          /* promote index to seq_t for arithmetic */

    if ((n & 1) == 0)            /* even n */
        return t * 2 + 1;
    else                         /* odd n (n>=3) */
        return t * 2 + 2;
}
