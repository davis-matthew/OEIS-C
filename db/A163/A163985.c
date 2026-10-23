/*
 * A163985  Sum of all isolated parts of all partitions of n.
 * https://oeis.org/A163985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163985: Sum of all isolated parts of all partitions of n
   Formula:
     a(n) = n                 for n < 3
     a(n) = 2*n - 1           for n >= 3
   (The closed form given in the OEIS entry.) */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;             /* convert index to sequence type */
    if (ns < 3)
        return ns;                   /* a(0)=0, a(1)=1, a(2)=2 */
    return 2*ns - 1;                /* a(n)=2*n-1 for n>=3 */
}
