/*
 * A166601  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 20, a(2) = 380, a(3) = 7220, a(4) = 137180,
 *            a(5) = 2606420, a(6) = 49521980, a(7) = 940917620, a(8) =
 *            17877434780, a(9) = 339671260820, a(10) = 6453753955580, a(11) =
 *            122621325156020, a(12) = 2329805177964190, a(13) =
 *            44266298381316000 For n ≥ 14: a(n) = 19·a(n−1) – 189·a(n−12)
 *            +...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;          /* allocated size of cache   */
    static idx_t max_computed = 0;       /* largest n for which a(n) is computed */

    /* initial values a(0)..a(13) */
    static const seq_t init[14] = {
        (seq_t)1ULL,
        (seq_t)20ULL,
        (seq_t)380ULL,
        (seq_t)7220ULL,
        (seq_t)137180ULL,
        (seq_t)2606420ULL,
        (seq_t)49521980ULL,
        (seq_t)940917620ULL,
        (seq_t)17877434780ULL,
        (seq_t)339671260820ULL,
        (seq_t)6453753955580ULL,
        (seq_t)122621325156020ULL,
        (seq_t)2329805177964190ULL,
        (seq_t)44266298381316000ULL
    };

    /* initialize cache on first call */
    if (!cache) {
        cache_cap = 14;
        cache = alloc_seq_array(cache_cap);
        if (!cache) return (seq_t)0;
        for (idx_t i = 0; i < 14; ++i) cache[i] = init[i];
        max_computed = 13;
    }

    /* for n < 14 we already have the value */
    if (n < 14) return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;        /* double until large enough */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return (seq_t)0;
        for (idx_t i = 0; i <= max_computed; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* compute new values up to requested index n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* recurrence: a(k) = 19*a(k-1) - 189*a(k-12) + 171*a(k-13) */
        seq_t term1 = (seq_t)19 * cache[k - 1];
        seq_t term2 = (seq_t)189 * cache[k - 12];
        seq_t term3 = (seq_t)171 * cache[k - 13];
        cache[k] = term1 - term2 + term3;
    }
    max_computed = n;
    return cache[n];
}
