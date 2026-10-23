/*
 * A111121  a(n) = a(n-3)^3 + a(n-2)^2 + a(n-1); a(1) = -1, a(2) = 0, a(3) =
 *          1.
 * https://oeis.org/A111121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-3)^3 + a(n-2)^2 + a(n-1); a(1) = -1, a(2) = 0, a(3) =
 *            1
 * Range:     i8 n <= 10, i16 n <= 12, i32 n <= 13, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static memoization storage */
static seq_t *memo = NULL;       /* array of computed values (1‑based) */
static idx_t memo_cap = 0;       /* current capacity of memo (in elements) */
static idx_t memo_size = 0;      /* last index computed (>=3 after init) */

/* compute the n‑th term of A111121 */
seq_t a(idx_t n) {
    if (n < 1) return 0;           /* out of bounds, never used in tests */

    /* lazy initialization of memo with the 3 given starting values */
    if (memo == NULL) {
        memo_cap = 4;              /* indices 0..3 (0 unused) */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) exit(1);        /* out of memory */
        memo[1] = (seq_t)(-1);
        memo[2] = (seq_t)0;
        memo[3] = (seq_t)1;
        memo_size = 3;
    }

    /* grow the memo array if we need more terms */
    if (n > memo_size) {
        while (memo_cap <= n) memo_cap *= 2;   /* double until large enough */
        memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        if (!memo) exit(1);

        /* compute missing terms up to n */
        for (idx_t i = memo_size + 1; i <= n; ++i) {
            seq_t t1 = memo[i-3];
            seq_t t2 = memo[i-2];
            seq_t t3 = memo[i-1];
            memo[i] = t1 * t1 * t1 + t2 * t2 + t3;
        }
        memo_size = n;
    }

    return memo[n];
}
