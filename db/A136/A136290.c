/*
 * A136290  a(0)=1, a(1)=3, a(2)=9, a(3)=12, a(4)=15; thereafter a(n) = a(n-1)
 *          + a(n-3) - a(n-4).
 * https://oeis.org/A136290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4); with a(0)=1, a(1)=3, a(2)=9,
 *            a(3)=12, a(4)=15
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base cases for n < 5 */
    static const seq_t base[5] = {1, 3, 9, 12, 15};

    /* memoization array */
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;

    /* trivial case */
    if (n < 5) {
        return base[(size_t)n];
    }

    /* initialise cache if needed */
    if (memo_cap == 0) {
        memo_cap = 5;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;              /* allocation failed */
        for (size_t i = 0; i < 5; ++i) {
            memo[i] = base[i];
        }
    }

    /* grow cache to reach n */
    if (n >= memo_cap) {
        size_t old_cap = memo_cap;
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        memo = tmp;
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            memo[i] = memo[i-1] + memo[i-3] - memo[i-4];
        }
        memo_cap = new_cap;
    }

    return memo[(size_t)n];
}
