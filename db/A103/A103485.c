/*
 * A103485  Expansion of (7 +4*x -5*x^2 -7*x^3) / ((1-x)*(1-x^2-x^3)).
 * https://oeis.org/A103485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4), a(0) = 7, a(1) = 11, a(2) = 13,
 *            a(3) = 17
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 71, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static array to memoize computed values */
    static seq_t *memo = NULL;
    static idx_t cap = 0;   /* allocated capacity */
    static idx_t sz  = 0;   /* number of computed elements (sz == cap may not hold) */

    /* Handle the first four terms directly */
    if (n < 4) {
        static const seq_t init[4] = {7, 11, 13, 17};
        return init[n];
    }

    /* First call: allocate and initialize base terms */
    if (memo == NULL) {
        cap = 4;
        memo = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!memo) return 0;            /* allocation failure – unlikely in tests */

        memo[0] = 7;
        memo[1] = 11;
        memo[2] = 13;
        memo[3] = 17;
        sz = 4;
    }

    /* Extend the memo array if requested n is outside current capacity */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;   /* exponential growth */
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        memo = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = sz; i <= n; ++i) {
        memo[i] = memo[i-1] + memo[i-2] - memo[i-4];
    }
    sz = n + 1;

    return memo[n];
}
