/*
 * A212691  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w+|x-y|<=|x-z|+|y-z|.
 * https://oeis.org/A212691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(14*n-7+3*(-1)^n)/24
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    if (n_seq < 2) /* a(0)=0, a(1)=0 */
        return 0;

    /* Compute the inner term 14*n-7+3*(-1)^n without negative values */
    seq_t inner = (n_seq & 1)
                   ? ((seq_t)14 * n_seq - (seq_t)10)  /* odd n : 14*n -10 */
                   : ((seq_t)14 * n_seq - (seq_t)4);   /* even n : 14*n -4 */

    seq_t part1 = n_seq * (n_seq - 1);
    seq_t result = part1 * inner / (seq_t)24;
    return result;
}
