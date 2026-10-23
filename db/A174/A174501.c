/*
 * A174501  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A003499(n))
 *          ), where A003499(n) = (3+sqrt(8))^n + (3-sqrt(8))^n.
 * https://oeis.org/A174501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(2n-1) = 1, a(2n) = A003499(n) - 2, where A003499(n) satisfies
 *            the recurrence B(0)=2, B(1)=6, B(k)=6*B(k-1)-B(k-2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 1)          /* odd indices */
        return (seq_t)1;

    idx_t k = n >> 1;          /* even index: k = n/2 */

    /* Lazy static cache of B(k) values */
    static seq_t *B = NULL;
    static idx_t max_k = 0;

    /* initialise cache for k = 0,1 */
    if (B == NULL) {
        B = (seq_t *)malloc(2 * sizeof(seq_t));
        if (B == NULL)          /* allocation failed – give up */
            return (seq_t)0;
        B[0] = (seq_t)2;
        B[1] = (seq_t)6;
        max_k = 1;
    }

    /* extend cache as needed */
    if (k > max_k) {
        seq_t *newB = (seq_t *)realloc(B, (k + 1) * sizeof(seq_t));
        if (newB == NULL)       /* allocation failed – give up */
            return (seq_t)0;
        B = newB;
        for (idx_t i = max_k + 1; i <= k; ++i)
            B[i] = (seq_t)6 * B[i - 1] - B[i - 2];
        max_k = k;
    }

    /* a(2n) = B(k) - 2 */
    return B[k] - (seq_t)2;
}
