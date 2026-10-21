/*
 * A204283  Number of (n+2) X 9 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204283
 * Recurrence for n > 8:  a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6)
 * initial values (n = 1..8):
 *   29731, 1883, 1990, 1914, 2168, 2860, 3664, 6784
 * This function memoizes computed terms; it works for any order of calls.
 */
#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[8] = {
    (seq_t)29731,
    (seq_t)1883,
    (seq_t)1990,
    (seq_t)1914,
    (seq_t)2168,
    (seq_t)2860,
    (seq_t)3664,
    (seq_t)6784
};

static seq_t *cache      = NULL;   /* cached terms, zero-based index */
static size_t cache_len  = 0;       /* number of cached terms */
static size_t cache_cap  = 0;       /* allocated capacity */

/* enlarge cache if necessary so that it can hold at least n terms */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* store initial values into the cache if they are not already present */
static void fill_initial_values(void)
{
    if (cache_len >= 8)
        return;
    size_t needed = 8 - cache_len;
    for (size_t i = cache_len; i < 8; ++i)
        cache[i] = init_vals[i];
    cache_len = 8;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined below offset; safe */

    ensure_capacity(n);
    if (cache_len < n) {
        /* if we don't yet have all initial values, add them */
        fill_initial_values();
        /* compute any remaining terms up to n */
        for (idx_t k = cache_len; k < n; ++k) {
            if (k < 8) {                 /* should already be filled */
                cache[k] = init_vals[k];
            } else {
                /* recurrence: a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) */
                seq_t term =
                    ((seq_t)3 * cache[k - 1]) -
                    ((seq_t)6 * cache[k - 4]) +
                    ((seq_t)4 * cache[k - 6]);
                cache[k] = term;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
