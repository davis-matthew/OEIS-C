/*
 * A230348  Number of permutations of [2n+8] in which the longest increasing
 *          run has length n+8.
 * https://oeis.org/A230348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+18*n^2+90*n+89) * (2*n+8)!/(n+10)! (n>0), a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>        /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle base cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    /* Compute the polynomial piece first */
    seq_t tn = (seq_t)n;
    seq_t poly = tn * tn * tn               /* n^3 */
              + (seq_t)18 * tn * tn          /* 18 n^2 */
              + (seq_t)90 * tn               /* 90 n */
              + (seq_t)89;                  /* +89 */

    /* Compute the factorial ratio as a product :
       (2n+8)!/(n+10)! = product_{k=n+11}^{2n+8} k   (for n>=2) */
    seq_t prod = (seq_t)1;
    idx_t start = n + (idx_t)11;
    idx_t end   = 2 * n + (idx_t)8;

    for (idx_t k = start; k <= end; ++k)
        prod *= (seq_t)k;

    return prod * poly;
}
