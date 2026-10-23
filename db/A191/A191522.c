/*
 * A191522  Number of valleys in all left factors of Dyck paths of length n. A
 *          valley is a (1,-1)-step followed by a (1,1)-step.
 * https://oeis.org/A191522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)/2) * binomial(n-1, floor((n-1)/2)+1) (for
 *            n>0, a(0)=0)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = (n - 1) / 2;          /* floor((n-1)/2) */
    if (m == 0)                     /* handles n = 1,2 */
        return 0;

    seq_t bin = binomial(n - 1, m + 1);   /* C(n-1, m+1) */
    return (seq_t)m * bin;                /* floor((n-1)/2) * C(n-1, m+1) */
}
