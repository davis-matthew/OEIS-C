/*
 * A230251  Number of permutations of [2n+1] in which the longest increasing
 *          run has length n+1.
 * https://oeis.org/A230251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5 + 6*n + 4*n^2 + n^3) * (2*n+1)! / (n+3)! for n>0,
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)41;

    /* Compute the polynomial part: n^3 + 4*n^2 + 6*n + 5 */
    seq_t nn   = (seq_t)n;
    seq_t poly = nn * nn * nn;        /* n^3 */
    poly += 4 * nn * nn;              /* +4*n^2 */
    poly += 6 * nn;                   /* +6*n */
    poly += 5;                        /* +5 */

    /* Compute the factorial ratio (2*n+1)! / (n+3)! =
       product of integers from n+4 to 2*n+1 (for n>=3) */
    seq_t prod = (seq_t)1;
    idx_t start = n + 4;
    idx_t end   = 2 * n + 1;
    for (idx_t k = start; k <= end; ++k) {
        prod *= (seq_t)k;
    }

    return poly * prod;
}
