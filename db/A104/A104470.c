/*
 * A104470  Tribonacci equivalent of mousetrap sequence (A002467).
 * https://oeis.org/A104470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=a(1)=a(2)=1; for n>2: a(n)=n*(a(n-1)+a(n-2)+a(n-3))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 426 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *memo   = NULL;   /* array of computed terms */
    static idx_t memo_cap = 0;     /* allocated capacity (elements) */
    static idx_t memo_len = 0;     /* number of values already computed */

    /* ensure sufficient capacity for indices up to n */
    if (n + 1 > memo_cap) {
        idx_t new_cap = memo_cap ? memo_cap << 1 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp;
        if (memo == NULL)
            tmp = seq_malloc(new_cap);
        else
            tmp = seq_realloc(memo, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute any missing terms */
    if (n + 1 > memo_len) {
        for (idx_t i = memo_len; i <= n; ++i) {
            if (i < 3)
                memo[i] = 1;
            else {
                seq_t sum = memo[i-1] + memo[i-2] + memo[i-3];
                memo[i] = ((seq_t)i) * sum;
            }
        }
        memo_len = n + 1;
    }

    return memo[n];
}
