/*
 * A038237  Second line of 4-wave sequence A038197.
 * https://oeis.org/A038237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) + 7*a(n-6) - (15*a(n-4) + a(n-8)) for n >= 8
 *            Initial values: a(0) = 1, a(1) = 3, a(2) = 7, a(3) = 26, a(4) =
 *            56, a(5) = 216, a(6) = 462, a(7) = 1791
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

/* Memoisation cache */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* Initial values for n = 0..7 */
static const seq_t init_vals[8] = {
    (seq_t)1,  (seq_t)3,  (seq_t)7,  (seq_t)26,
    (seq_t)56, (seq_t)216, (seq_t)462, (seq_t)1791
};

seq_t a(idx_t n)
{
    /* Allocate or grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms up to n that have not yet been computed */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 8) {
            val = init_vals[i];
        } else {
            /* Recurrence: a_i = 10*a_{i-2} + 7*a_{i-6} - (15*a_{i-4} + a_{i-8}) */
            seq_t t1 = 10 * cache[i - 2];
            t1 += 7 * cache[i - 6];
            seq_t t2 = 15 * cache[i - 4];
            t2 += cache[i - 8];
            val = t1 - t2;          /* final value is non‑negative */
        }
        cache[i] = val;
    }

    /* Update length after new entries have been added */
    if (n >= cache_len) cache_len = n + 1;

    return cache[n];
}
