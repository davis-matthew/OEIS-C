/*
 * A224408  Number of n X 7 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2) + 18*a(n-3) - 4*a(n-4) + 2*a(n-5) +
 *            8*a(n-6) + 32*a(n-7) Initial values: a(1)=29, a(2)=267,
 *            a(3)=1568, a(4)=7490, a(5)=32814, a(6)=139638, a(7)=590254,
 *            a(8)=2496332 (The recurrence holds for n>8.)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;   /* array of terms, 1‑based */
static size_t cache_cap  = 0;      /* allocated capacity (index 0 unused) */
static size_t cache_len  = 0;      /* highest computed index */

/* N.B. All arithmetic is performed in seq_t.  The utility functions
   seq_alloc_new_array and seq_realloc allocate arrays of seq_t objects
   correctly for both POD and arbitrary‑precision integer types. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range (offset is 1) */

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;          /* index 0 will remain unused */
    if (need > cache_cap) {
        cache = seq_realloc(cache, cache_cap, need);
        cache_cap = need;
    }

    /* compute any terms that have not yet been cached */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            /* initial terms (indices 1 … 8) */
            static const seq_t init[8] = {
                (seq_t)29,    (seq_t)267,    (seq_t)1568,
                (seq_t)7490,  (seq_t)32814,  (seq_t)139638,
                (seq_t)590254,(seq_t)2496332
            };
            cache[i] = init[i - 1];
        } else {
            /* recurrence for i > 8 */
            cache[i] = (seq_t)8   * cache[i - 1]
                     - (seq_t)20  * cache[i - 2]
                     + (seq_t)18  * cache[i - 3]
                     - (seq_t)4   * cache[i - 4]
                     + (seq_t)2   * cache[i - 5]
                     + (seq_t)8   * cache[i - 6]
                     + (seq_t)32  * cache[i - 7];
        }
    }
    cache_len = (size_t)n;

    return cache[n];
}
