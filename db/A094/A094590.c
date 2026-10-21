/*
 * A094590  a(1) = 1; a(n+1) = a(n) * k(n), where k(n) is the number of
 *          elements of {a(j)}, 1<=j<=n, which are <= n.
 * https://oeis.org/A094590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = a(n-1) * k(n-1)
 * where k(m) is the number of terms a(j) (1 <= j <= m) that are <= m.
 * The sequence starts with a(1) = 1.
 * Recurrence: for n > 1,
 *     a(n) = a(n-1) * k(n-1)
 *     k(m) = #{ j | 1 <= j <= m && a(j) <= m }   */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Return the nth term of OEIS A094590 */
seq_t a(idx_t n) {
    /* Persistent cache of computed terms (1‑indexed). */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;      /* highest index currently cached */
    static idx_t pos_count = 0;  /* number of cached a[j] <= max_n */

    /* Basic initialisation: compute a(1) = 1 */
    if (max_n == 0) {
        cache = seq_alloc_new_array(2);    /* indices 0..1 (0 unused) */
        cache[1] = (seq_t)1;
        max_n = 1;
        pos_count = 1;  /* a(1) <= 1 */
    }

    /* Grow the cache up to n if needed */
    if (n > max_n) {
        idx_t new_size = n + 1;            /* allocate up to index n (0 unused) */
        seq_t *new_cache = seq_alloc_new_array(new_size);

        /* Copy existing computed terms */
        for (idx_t i = 1; i <= max_n; ++i)
            new_cache[i] = cache[i];

        seq_free_array(cache);
        cache = new_cache;

        /* Extend the sequence */
        for (idx_t i = max_n; i < n; ++i) {
            /* Update pos_count for current index i */
            while ((pos_count + 1) <= i && cache[pos_count + 1] <= (seq_t)i)
                ++pos_count;

            seq_t k = (seq_t)pos_count;
            cache[i + 1] = cache[i] * k;
        }

        max_n = n;

        /* Ensure pos_count matches the new maximum index */
        while ((pos_count + 1) <= max_n && cache[pos_count + 1] <= (seq_t)max_n)
            ++pos_count;
    }

    return cache[n];
}
