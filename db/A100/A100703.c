/*
 * A100703  (T(n-1) + T(n-2)) + T(n-1)*T(n-2) where T(0)=3, T(1)=5 and n >= 2.
 * https://oeis.org/A100703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) + a(n-2)) + a(n-1)*a(n-2) with a(0)=3, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache so calls are not required to be monotonic */
    static seq_t *memo = NULL;
    static idx_t  size  = 0;

    /* trivial cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;

    /* initialise cache with first two terms if necessary */
    if (size == 0) {
        size = 2;
        memo = (seq_t *)calloc((size_t)size, sizeof(seq_t));
        if (!memo) return (seq_t)0;       /* out‑of‑memory: give 0 */
        memo[0] = (seq_t)3;
        memo[1] = (seq_t)5;
    }

    /* extend cache if required */
    if (size <= n) {
        seq_t *tmp = (seq_t *)realloc(memo, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* out‑of‑memory: give 0 */
        memo = tmp;
        /* compute new terms */
        for (idx_t i = size; i <= n; ++i) {
            seq_t a1 = memo[i-1];
            seq_t a2 = memo[i-2];
            memo[i] = (a1 + a2) + a1 * a2;
        }
        size = n + 1;
    }

    return memo[n];
}
