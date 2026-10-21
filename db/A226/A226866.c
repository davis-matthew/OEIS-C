/*
 * A226866  Number of n X 2 (-1,0,1) arrays of determinants of 2 X 2 subblocks
 *          of some (n+1) X 3 binary array with rows and columns of the latter
 *          in lexicographically nondecreasing order.
 * https://oeis.org/A226866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 35*n^3 + 84*n)/40 + 1
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 92, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1)-15*a(n-2)+20*a(n-3)-15*a(n-4)+6*a(n-5)-a(n-6) (for n>6) */
/* Initial terms: a(1)=4, a(2)=13, a(3)=37, a(4)=91, a(5)=199, a(6)=397 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* value outside the OEIS offset */
    seq_t ns = (seq_t)n;        /* cast index to seq_t */
    seq_t n2 = ns * ns;         /* n^2 */
    seq_t n3 = n2 * ns;         /* n^3 */
    seq_t n5 = n3 * n2;         /* n^5 = n^3 * n^2 */

    /* numerator = n^5 + 35*n^3 + 84*n */
    seq_t numerator = n5 + (seq_t)35 * n3 + (seq_t)84 * ns;

    /* division by 40 is exact for all integer n ≥ 1 */
    seq_t res = numerator / (seq_t)40 + (seq_t)1;
    return res;
}
