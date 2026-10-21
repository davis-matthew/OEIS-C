/*
 * A224025  Number of 3 X n 0..3 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) -45*a(n-2) +120*a(n-3) -210*a(n-4) +252*a(n-5)
 *                -210*a(n-6) +120*a(n-7) -45*a(n-8) +10*a(n-9) - a(n-10)
 * for n>11.  Initial values (n=1..11):
 * 64, 1000, 6796, 32523, 122523, 387729, 1074167, 2679260,
 * 6137666, 13104218, 26368076. */

#include <stddef.h>
#include <stdlib.h>

/* Initial values for n = 1 .. 11 (index 0 unused). */
static const seq_t init_vals[12] = {
    0ULL,                  /* unused */
    (seq_t)64,
    (seq_t)1000,
    (seq_t)6796,
    (seq_t)32523,
    (seq_t)122523,
    (seq_t)387729,
    (seq_t)1074167,
    (seq_t)2679260,
    (seq_t)6137666,
    (seq_t)13104218,
    (seq_t)26368076
};

static seq_t *cache   = NULL;   /* 1‑based cached values */
static idx_t  cache_len = 0;    /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined but safe */

/* Initialise or extend the cache as needed. */
    if (cache_len == 0) {
        size_t cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed */
        for (idx_t i = 1; i <= 11; ++i) cache[i] = init_vals[i];
        cache_len = 11;
    } else if (n > cache_len) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = 0;
            val += (seq_t)10 * cache[i-1];
            val -= (seq_t)45 * cache[i-2];
            val += (seq_t)120 * cache[i-3];
            val -= (seq_t)210 * cache[i-4];
            val += (seq_t)252 * cache[i-5];
            val -= (seq_t)210 * cache[i-6];
            val += (seq_t)120 * cache[i-7];
            val -= (seq_t)45 * cache[i-8];
            val += (seq_t)10 * cache[i-9];
            val -= (seq_t)1 * cache[i-10];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
