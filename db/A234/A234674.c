/*
 * A234674  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234674: Number of (n+1)×(1+1) 0..5 arrays with every 2×2 subblock having
 * its diagonal sum differing from its antidiagonal sum by 5.
 * Empirical recurrence (valid for n ≥ 5):
 *   a(n) = 22·a(n‑2) − 120·a(n‑4)
 * Initial values:
 *   a(1)=112, a(2)=404, a(3)=1264, a(4)=4568
 *
 * This function memoises calculated values up to the largest n requested.
 * It works with any integer type seq_t supplied by the build, including
 * arbitrary‑precision integer classes for the C++ build.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle indices below 1 – the sequence is defined for n≥1 */
    if (n < 1) return (seq_t)0;

    /* static storage for memoised values: index 0 is unused (set 0), indices 1..n used */
    static seq_t *cache = NULL;
    static size_t capped = 0;          /* number of elements allocated (max index + 1) */

    /* initialise base values on first use */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* memory allocation failed – return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)112;
        cache[2] = (seq_t)404;
        cache[3] = (seq_t)1264;
        cache[4] = (seq_t)4568;
        capped  = 5;                  /* indices 0..4 are initialised */
    }

    /* extend cache if requested n is beyond current capacity */
    if ((idx_t)capped <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* realloc failed – return 0 */
        cache = tmp;
        /* compute new entries using the recurrence */
        for (idx_t i = capped; i <= n; ++i) {
            if (i >= 5) {
                cache[i] = 22 * cache[i - 2] - 120 * cache[i - 4];
            } else { /* should not happen, but keep for safety */
                cache[i] = cache[i];
            }
        }
        capped = new_cap;
    }

    return cache[n];
}
