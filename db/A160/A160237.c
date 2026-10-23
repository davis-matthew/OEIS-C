/*
 * A160237  Numerator of Hermite(n, 6/29).
 * https://oeis.org/A160237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 6/29) Recurrence: a(0) = 1, a(1) = 12
 *            a(n) = 12 * a(n-1) - 1682 * (n-1) * a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: memo[i] holds a(i) */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;     /* number of entries currently stored */
    static size_t memo_cap = 0;     /* allocated capacity */

    /* grow cache to at least n+1 entries */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap : 4;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!new_mem) {
            /* allocation failed; abort */
            abort();
        }
        memo = new_mem;
        memo_cap = new_cap;
    }

    /* initialise base terms if missing */
    if (memo_len == 0) {
        memo[0] = (seq_t)1;
        memo_len = 1;
    }
    if (memo_len == 1 && n >= 1) {
        memo[1] = (seq_t)12;
        memo_len = 2;
    }

    /* compute missing terms up to n */
    if ((size_t)n + 1 > memo_len) {
        for (size_t i = memo_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                memo[i] = (seq_t)1;
            } else if (i == 1) {
                memo[i] = (seq_t)12;
            } else {
                seq_t coef = (seq_t)1682 * (seq_t)(i - 1);
                memo[i] = 12 * memo[i - 1] - coef * memo[i - 2];
            }
        }
        memo_len = (size_t)n + 1;
    }

    return memo[(size_t)n];
}
