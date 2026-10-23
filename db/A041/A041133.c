/*
 * A041133  Denominators of continued fraction convergents to sqrt(75).
 * https://oeis.org/A041133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 52*a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=50, a(5)=53, a(6)=103, a(7)=156
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;    /* number of terms stored in memo (>= memo_cap) */
    static idx_t memo_cap = 0;    /* allocated capacity of memo in terms */

    /* Initial seven terms */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)50, (seq_t)53, (seq_t)103, (seq_t)156
    };

    /* Return a hard‑wired value for n < 8 */
    if (n < 8) {
        return init[n];
    }

    /* Allocate initial cache if needed */
    if (memo == NULL) {
        memo_cap = 16;   /* start with a small capacity */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (memo == NULL) {
            abort();
        }
        for (idx_t i = 0; i < 8; ++i) {
            memo[i] = init[i];
        }
        memo_len = 8;
    }

    /* Grow cache if the requested index is outside the capacity */
    if (n >= memo_cap) {
        while (memo_cap <= n) {
            memo_cap <<= 1;          /* double the capacity */
        }
        seq_t *tmp = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        memo = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        memo[i] = (seq_t)52 * memo[i - 4] - memo[i - 8];
    }
    memo_len = n + 1;

    return memo[n];
}
