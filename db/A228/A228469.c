/*
 * A228469  a(n) = 6*a(n-2) + a(n-4), where a(0) = 2, a(1) = 8, a(2) = 13,
 *          a(3) = 49.
 * https://oeis.org/A228469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + a(n-4), a(0)=2, a(1)=8, a(2)=13, a(3)=49
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)49;

    /* For n ≥ 4 the recurrence splits into two independent linear
       recurrences for the even and odd indices.  We compute the
       appropriate subsequence iteratively, requiring only two
       previous terms. */
    if ((n & 1) == 0) {                     /* even n = 2*k */
        idx_t k = n >> 1;                   /* k = n/2 */
        seq_t prev2 = (seq_t)2;             /* a(0)   */
        seq_t prev1 = (seq_t)13;            /* a(2)   */
        idx_t i;
        for (i = 2; i <= k; ++i) {          /* compute a(2*i) */
            seq_t cur = 6 * prev1 + prev2;  /* a(2*i) = 6*a(2*(i-1)) + a(2*(i-2)) */
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    } else {                                /* odd n = 2*k+1 */
        idx_t k = (n - 1) >> 1;             /* k = (n-1)/2 */
        seq_t prev2 = (seq_t)8;             /* a(1)   */
        seq_t prev1 = (seq_t)49;            /* a(3)   */
        idx_t i;
        for (i = 2; i <= k; ++i) {          /* compute a(2*i+1) */
            seq_t cur = 6 * prev1 + prev2;  /* a(2*i+1) = 6*a(2*(i-1)+1) + a(2*(i-2)+1) */
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    }
}
