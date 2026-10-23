/*
 * A101514  Shifts one place left under the square binomial transform
 *          (A008459): a(0) = 1, a(n) = Sum_{k=0..n-1} C(n-1,k)^2*a(k).
 * https://oeis.org/A101514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = Sum_{k=0..n-1} C(n-1,k)^2 * a(k) for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* resize_array is provided by the build system. */
#include "../utils/resize_array.h"

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;

    /* initialise memo[0] = 1 */
    if (memo == NULL) {
        memo = resize_array(NULL, 0, 1);
        /* in case allocation fails */
        if (memo == NULL) return 0;
        memo[0] = 1;
        memo_len = 1;
    }

    /* extend the memo table up to n if necessary */
    if (n >= memo_len) {
        memo = resize_array(memo, memo_len, n + 1);
        for (idx_t i = memo_len; i <= n; ++i) {
            seq_t sum = 0;
            seq_t C   = 1;                   /* C(i-1,0) */
            for (idx_t k = 0; k < i; ++k) {
                /* prod = a(k) * C^2 */
                seq_t prod = memo[k] * C;
                prod = prod * C;
                sum += prod;

                /* update C to C(i-1,k+1) */
                if (k + 1 < i) {
                    seq_t num = (seq_t)(i - 1 - k);   /* (i-1 - k)  */
                    seq_t den = (seq_t)(k + 1);       /* (k+1)      */
                    C = C * num / den;
                }
            }
            memo[i] = sum;
        }
        memo_len = n + 1;
    }

    return memo[n];
}
