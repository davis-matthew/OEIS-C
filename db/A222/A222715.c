/*
 * A222715  The number of binary pattern classes in the (2,n)-rectangular grid
 *          with 5 '1's and (2n-5) '0's: two patterns are in same class if one
 *          of them can be obtained by a reflection or 180-degree rotation of
 *          the other.
 * https://oeis.org/A222715
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n,5) / 4 for n even a(n) = (C(2n,5) + 2*C(n-1,2)) / 4
 *            for n odd (where C(x,y) is the binomial coefficient)
 * Range:     u8 none, u16 n <= 5, u32 n <= 140, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* below offset – value irrelevant */

    /* compute C(2n,5) = ((2n)*(2n-1)*(2n-2)*(2n-3)*(2n-4))/120 */
    seq_t two_n = (seq_t)n * (seq_t)2;
    seq_t prod  = two_n;
    prod *= (two_n - (seq_t)1);
    prod *= (two_n - (seq_t)2);
    prod *= (two_n - (seq_t)3);
    prod *= (two_n - (seq_t)4);
    prod /= (seq_t)120;                   /* exact division */

    /* compute C(n-1,2) = ((n-1)*(n-2))/2 */
    seq_t c = (seq_t)(n - (idx_t)1);
    c *= (seq_t)(n - (idx_t)2);
    c /= (seq_t)2;

    /* assemble final result */
    if ((n & (idx_t)1) == 0)              /* even n */
        return prod / (seq_t)4;
    else                                  /* odd n */
        return (prod + (c * (seq_t)2)) / (seq_t)4;
}
