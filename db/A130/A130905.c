/*
 * A130905  Expansion of e.g.f. exp(x^2 / 2) / (1 - x).
 * https://oeis.org/A130905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A130905
   a(n) = n*a(n-1) + (n-1)*a(n-2) - (n-2)*(n-1)*a(n-3)   for n >= 3
   a(0) = 1, a(1) = 1, a(2) = 3
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *memo = NULL;
    static idx_t memo_len  = 0;  /* number of computed elements */
    static idx_t capacity  = 0;  /* allocated capacity */

    /* initialise the first three values once */
    if (memo_len == 0) {
        memo_len = 3;
        capacity = 3;
        memo = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        if (!memo) return 0;          /* allocation failed */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)3;
    }

    /* if requested value already cached, return it */
    if (n < memo_len)
        return memo[n];

    /* grow the cache if necessary so that it contains at least n+1 entries */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap <= n)
            new_cap <<= 1;   /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        memo = tmp;
        capacity = new_cap;
    }

    /* compute missing values from memo_len up to n (inclusive) */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i == 0 || i == 1 || i == 2) { /* base values already set */
            continue;
        }

        seq_t term = memo[i-1];                       /* i*a(i-1) */
        term *= (seq_t)i;

        term += memo[i-2] * (seq_t)(i-1);             /* + (i-1)*a(i-2) */

        term -= memo[i-3] * (seq_t)(i-2) * (seq_t)(i-1); /* - (i-2)*(i-1)*a(i-3) */

        memo[i] = term;
    }

    /* update length of known values */
    memo_len = n + 1;

    return memo[n];
}
