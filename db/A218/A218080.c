/*
 * A218080  Number of nX4 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random 0..1 nX4 array.
 * https://oeis.org/A218080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +9*a(n-2) -4*a(n-3) -37*a(n-4) -13*a(n-5)
 *            +48*a(n-6) +39*a(n-7) -25*a(n-8) -18*a(n-9) -6*a(n-10)
 *            -3*a(n-11) -5*a(n-12) +4*a(n-13) Initial terms: (n, a(n)) for n
 *            = 1..13 as listed in OEIS. The recurrence holds for all n >= 14
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

static void ensure_capacity(idx_t n)
{
    if (cache_max >= n) return;
    size_t new_cap = (size_t)n + 1; /* index 0 unused */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = new_cap;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    /* initialize known values if needed */
    if (cache_max == 0) {
        static const seq_t init_vals[13] = {
            (seq_t)6, (seq_t)18, (seq_t)78, (seq_t)342, (seq_t)1508,
            (seq_t)6606, (seq_t)29136, (seq_t)128220, (seq_t)564548,
            (seq_t)2485936, (seq_t)10945378, (seq_t)48194280,
            (seq_t)212204122
        };
        for (int i = 1; i <= 13; ++i) {
            cache[i] = init_vals[i-1];
        }
        cache_max = 13;
    }
    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* sums of positive and negative contributions separately */
        seq_t sum_pos =  (seq_t)3 * cache[i-1]
                      + (seq_t)9 * cache[i-2]
                      + (seq_t)48 * cache[i-6]
                      + (seq_t)39 * cache[i-7]
                      + (seq_t)4 * cache[i-13];
        seq_t sum_neg =  (seq_t)4  * cache[i-3]
                      + (seq_t)37 * cache[i-4]
                      + (seq_t)13 * cache[i-5]
                      + (seq_t)25 * cache[i-8]
                      + (seq_t)18 * cache[i-9]
                      + (seq_t)6  * cache[i-10]
                      + (seq_t)3  * cache[i-11]
                      + (seq_t)5  * cache[i-12];
        cache[i] = sum_pos - sum_neg; /* guaranteed non‑negative */
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; behavior for 0 is irrelevant */
    if (n <= 13) {
        /* ensure cache is initialised */
        ensure_capacity(13);
        return cache[n];
    }
    ensure_capacity(n);
    return cache[n];
}
