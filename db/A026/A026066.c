/*
 * A026066  dot_product(n,n-1,...2,1)*(7,8,...,n,1,2,3,4,5,6).
 * https://oeis.org/A026066
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6) * (n^2 + 21n - 106) * n
 * Range:     u8 none, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7u)    /* behaviour for n < 7 is irrelevant */
        return (seq_t)0;
    seq_t tn  = (seq_t)n;
    seq_t tn2 = tn * tn;                /* n^2 */
    seq_t val = tn2 + 21u * tn;         /* n^2 + 21n */
    val -= 106u;                        /* n^2 + 21n - 106 */
    val *= tn;                          /* n * (n^2 + 21n - 106) */
    val /= 6u;                          /* divide by 6 */
    return val;
}
