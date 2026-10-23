/*
 * A110310  Expansion of (1-x+x^2)/((x^2+x+1)*(x^2+5*x+1)).
 * https://oeis.org/A110310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4) (n > 3) Initial
 *            values: a(0) = 1, a(1) = -7, a(2) = 36, a(3) = -173
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization buffer */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;   /* number of values stored (up to memo_size-1) */
    static idx_t memo_cap  = 0;   /* allocated capacity */

    /* initialise on first call */
    if (memo == NULL) {
        idx_t cap = n + 1;
        if (cap < 4) cap = 4;           /* ensure room for the first four terms   */
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;  /* allocation failure */

        memo[0] = (seq_t)1;
        memo[1] = (seq_t)-7;
        memo[2] = (seq_t)36;
        memo[3] = (seq_t)-173;
        memo_cap  = cap;
        memo_size = 4;                  /* we have computed indices 0..3          */
    }

    /* if requested term already computed */
    if (n < memo_size) return memo[n];

    /* grow the buffer if necessary */
    if (n + 1 > memo_cap) {
        idx_t new_cap = memo_cap << 1;          /* double */
        if (new_cap <= n + 1) new_cap = n + 1;  /* ensure enough space */
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* allocation failure */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute new values up to n using the 4‑term recurrence */
    for (idx_t i = memo_size; i <= n; ++i) {
        seq_t val = memo[i-1] * (seq_t)-6;
        val += memo[i-2] * (seq_t)-7;
        val += memo[i-3] * (seq_t)-6;
        val -= memo[i-4];                      /* note the minus sign */
        memo[i] = val;
    }
    memo_size = n + 1;

    return memo[n];
}
