/*
 * A204008  Symmetric matrix based on f(i,j) = max{3i+j-3,i+3j-3}, by
 *          antidiagonals.
 * https://oeis.org/A204008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the antidiagonal of the matrix M(i,j) = max{3*i + j
 *            - 3, i + 3*j - 3} for i,j >= 1; antidiagonals are taken in the
 *            order of increasing sum i+j
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Compute OEIS A204008 */
seq_t a(idx_t n)
{
    /* find t such that  (t+1)(t+2)/2 >= n > t(t+1)/2   */
    uint64_t t;
    {
        /* approximate using double, adjust by a few steps */
        long double d = sqrt((long double)(8.0L * (long double)n - 7.0L));
        t = (uint64_t)((d - 1.0L) / 2.0L);

        /* adjust downwards if overestimated */
        while (t > 0) {
            unsigned __int128 lhs = (unsigned __int128)t * (t + 1) / 2;
            if (lhs >= (unsigned __int128)n) {
                t--;
            } else {
                break;
            }
        }
        /* adjust upwards if underestimated */
        while ((unsigned __int128)(t + 1) * (t + 2) / 2 < (unsigned __int128)n) {
            t++;
        }
    }

    /* previous total elements before diagonal r = t+2 */
    unsigned __int128 prev_total = (unsigned __int128)t * (t + 1) / 2;
    unsigned __int128 offset128 = (unsigned __int128)n - prev_total - 1;
    uint64_t offset = (uint64_t)offset128;          /* offset fits in 64-bit */
    uint64_t i = offset + 1;                        /* row index */
    uint64_t r = t + 2;                             /* diagonal index = i+j */
    uint64_t j = r - i;                             /* column index */

    /* compute the two linear forms and take the maximum */
    unsigned __int128 val1 = (unsigned __int128)3 * i + j - 3;
    unsigned __int128 val2 = (unsigned __int128)i + (unsigned __int128)3 * j - 3;
    unsigned __int128 res  = val1 > val2 ? val1 : val2;

    return (seq_t)res;
}
