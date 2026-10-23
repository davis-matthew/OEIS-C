/*
 * A107233  An inverse Chebyshev transform of n^3.
 * https://oeis.org/A107233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*(n-2*k)^3
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    static idx_t memo_last = 0;

    /* initialise for n == 0 if necessary */
    if (memo_size == 0) {
        memo_size = 1;
        memo = (seq_t *)realloc(memo, memo_size * sizeof(seq_t));
        memo[0] = (seq_t)0;   /* a(0) = 0 */
        memo_last = 0;
    }

    if (n <= memo_last)
        return memo[n];

    /* grow table if needed */
    if (n >= memo_size) {
        idx_t new_size = memo_size ? memo_size * 2 : 16;
        while (new_size <= n)
            new_size *= 2;
        memo = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        memo_size = new_size;
    }

    /* compute missing values up to n */
    for (idx_t i = memo_last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = i / 2;
        seq_t binom = (seq_t)1;   /* C(i,0) */

        for (idx_t k = 0; k <= limit; ++k) {
            if (k > 0) {
                /* binom = binom * (i - k + 1) / k */
                seq_t num = binom * (seq_t)(i - k + 1);
                binom = num / (seq_t)k;
            }

            seq_t diff = (seq_t)(i - 2 * k);
            seq_t diff2 = diff * diff;
            seq_t diff3 = diff2 * diff;
            sum += binom * diff3;
        }

        memo[i] = sum;
    }

    memo_last = n;
    return memo[n];
}
