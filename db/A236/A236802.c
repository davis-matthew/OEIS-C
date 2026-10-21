/*
 * A236802  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          lower median minus the upper median minus the minimum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A236802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A236802  
    a(1) = 81, a(2) = 277, a(3) = 1033, a(4) = 4183, a(5) = 17481, a(6) = 75907
    Empirical recurrence (valid for n ≥ 7):
    a(n) = 5*a(n‑1) + 20*a(n‑2) – 130*a(n‑3) + 36*a(n‑4) + 600*a(n‑5) – 720*a(n‑6)  */
#include <stddef.h>
#include "../utils/seq_realloc.h"

static const seq_t init[7] = {
    (seq_t)0,
    (seq_t)81,
    (seq_t)277,
    (seq_t)1033,
    (seq_t)4183,
    (seq_t)17481,
    (seq_t)75907
};

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined case, return 0 for safety */
        return (seq_t)0;

    if (n <= 6)           /* return one of the known initial values */
        return init[n];

    /* static memoisation state */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   last_computed = 0;

    /* grow cache if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp)
            return (seq_t)0;    /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first six entries on first use */
    if (last_computed < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
        last_computed = 6;
    }

    /* compute successive terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t s = cache[i - 1] * 5;
        s += cache[i - 2] * 20;
        s -= cache[i - 3] * 130;
        s += cache[i - 4] * 36;
        s += cache[i - 5] * 600;
        s -= cache[i - 6] * 720;
        cache[i] = s;
    }

    last_computed = n;
    return cache[n];
}
