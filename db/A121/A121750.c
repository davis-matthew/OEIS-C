/*
 * A121750  Number of columns of even length in all deco polyominoes of height
 *          n. A deco polyomino is a directed column-convex polyomino in which
 *          the height, measured along the diagonal, is attained only in the
 *          last column.
 * https://oeis.org/A121750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1)+d(n-1)+(n-1)!*floor((n-1)/2) with d(1)=1,
 *            d(2)=0, d(2k)=3!+5!+...+(2k-1)!, d(2k+1)=-d(2k). The
 *            implementation is iterative and works for any unsigned integer
 *            type seq_t (uint8_t ... uint64_t, unsigned __int128) or
 *            arbitrary-precision class
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    seq_t a_prev = 0;          /* a(1) */
    seq_t fact = 1;            /* (i-1)! at start of loop */
    seq_t d_even = 0;          /* d value for the last even index processed */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute d(i-1) */
        seq_t temp = (seq_t)i * a_prev;                 /* i*a(i-1) */
        seq_t term3 = fact * ((i - 1) / 2);             /* (i-1)! * floor((i-1)/2) */
        seq_t term1term3 = temp + term3;                /* i*a(i-1) + term3 */

        seq_t a_cur;
        if (i - 1 == 1) {         /* d(1) = 1 */
            a_cur = term1term3 + (seq_t)1;
        } else if ((i - 1) % 2 == 0) { /* even index */
            a_cur = term1term3 + d_even;
        } else {                 /* odd index > 1: d = -d_even */
            a_cur = term1term3 - d_even;
        }

        /* Prepare for next iteration */
        a_prev = a_cur;

        /* Update d_even for the upcoming even index if necessary */
        if ((i & 1) == 0 && i >= 4) {
            /* fact currently holds (i-1)! which is odd factorial to add */
            d_even += fact;
        }

        /* Update factorial for next iteration */
        fact = fact * (seq_t)i;   /* now fact = i! */
    }

    return a_prev;
}
