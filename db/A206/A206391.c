/*
 * A206391  Number of n X 3 0..5 arrays with no element equal to another
 *          within a city block distance of two, and new values 0..5
 *          introduced in row major order.
 * https://oeis.org/A206391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 18*a(n-2) - 32*a(n-3) for n>5 * Initial
 *            values: a(1)=1, a(2)=4, a(3)=26, a(4)=206, a(5)=1592 * This
 *            implementation uses a memoised recurrence, storing results * in
 *            a static array to allow arbitrary call order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* initialise first five values on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..5 (index 0 unused) */
        cache = (seq_t *)malloc((5 + 2) * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out‑of‑memory fallback */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)206;
        cache[5] = (seq_t)1592;
        max_computed = 5;
    }

    /* for n <= 5 we already have the value */
    if (n <= 5) return cache[n];

    /* ensure cache large enough */
    if (n > max_computed) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 2) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* out‑of‑memory fallback */
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i) = 6*a(i-1) + 18*a(i-2) - 32*a(i-3) */
            seq_t val = (seq_t)6 * cache[i - 1];
            val += (seq_t)18 * cache[i - 2];
            val -= (seq_t)32 * cache[i - 3];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
