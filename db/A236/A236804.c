/*
 * A236804  Number of (n+1) X (3+1) 0..2 arrays with the maximum plus the
 *          lower median minus the upper median minus the minimum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A236804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) follows the empirical 11‑term linear recurrence
 *
 *   a(1..11) = 1033, 8825, 85929, 929447, 9558409, 107087939,
 *              1117775361, 12821027315, 135199336873, 1580326999835,
 *              16792477473969
 *
 *   a(n) = 24*a(n-1)
 *        - 55*a(n-2)
 *        - 2772*a(n-3)
 *        + 26568*a(n-4)
 *        - 71616*a(n-5)
 *        - 74996*a(n-6)
 *        + 644112*a(n-7)
 *        - 762032*a(n-8)
 *        - 588288*a(n-9)
 *        + 1612416*a(n-10)
 *        - 783360*a(n-11)           for n > 11
 *
 * The function below memoises the sequence in a static array and
 * extends it as needed.  It works for any order of calls, as
 * required.
 */

#include <stdlib.h>               /* malloc, free, abort */
#include <stdint.h>               /* intptr_t, uintptr_t */

static idx_t cache_cap = 0;          /* array capacity (>= last index + 1) */
static idx_t computed   = 0;          /* largest n with cache[n] filled */
static seq_t *cache     = NULL;       /* cache array, index 0 unused */

/* Base values for n = 1 … 11, indexed from 0 with a dummy at 0. */
static const seq_t base_vals[12] = {
    0ULL,                         /* dummy index 0 */
    1033ULL,                      /* a(1) */
    8825ULL,                      /* a(2) */
    85929ULL,                     /* a(3) */
    929447ULL,                    /* a(4) */
    9558409ULL,                   /* a(5) */
    107087939ULL,                 /* a(6) */
    1117775361ULL,                /* a(7) */
    12821027315ULL,               /* a(8) */
    135199336873ULL,              /* a(9) */
    1580326999835ULL,             /* a(10) */
    16792477473969ULL             /* a(11) */
};

/* Ensure the cache contains a(1) … a(n). */
static void _ensure_a(idx_t n)
{
    if (n < 1) return;

    /* Grow the cache if necessary. */
    if (cache_cap < n + 1) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failed */

        /* Copy previous data if any. */
        if (cache && cache_cap > 0) {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base values if not already present. */
    if (computed < 11) {
        idx_t upto = n < 11 ? n : 11;
        for (idx_t i = computed + 1; i <= upto; ++i)
            cache[i] = base_vals[i];
        computed = upto;
    }

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence when i >= 12 */
        seq_t term = 0;
        if (i-1 >= 1) term += 24ULL        * cache[i-1];
        if (i-2 >= 1) term -= 55ULL        * cache[i-2];
        if (i-3 >= 1) term -= 2772ULL      * cache[i-3];
        if (i-4 >= 1) term += 26568ULL     * cache[i-4];
        if (i-5 >= 1) term -= 71616ULL     * cache[i-5];
        if (i-6 >= 1) term -= 74996ULL     * cache[i-6];
        if (i-7 >= 1) term += 644112ULL    * cache[i-7];
        if (i-8 >= 1) term -= 762032ULL    * cache[i-8];
        if (i-9 >= 1) term -= 588288ULL    * cache[i-9];
        if (i-10 >= 1) term += 1612416ULL  * cache[i-10];
        if (i-11 >= 1) term -= 783360ULL   * cache[i-11];
        cache[i] = term;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _ensure_a(n);
    return cache[n];
}
