/*
 * A025633  Numbers of form 8^i*9^j, with i, j >= 0.
 * https://oeis.org/A025633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 65, u64 n <= 237, u128 n <= 903,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = smallest number of the form 8^i * 9^j (i,j ≥ 0)
 * sorted in increasing order.  The algorithm uses a two‑pointer
 * (ugly‑number) method that runs in O(n) time and with O(n) extra
 * space.  The sequence is memoised so that calls with arbitrary
 * order of n are still efficient. */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache for generated terms */
static seq_t *cache = NULL;           /* 1‑based array of terms      */
static size_t cache_cap = 0;          /* size allocated for cache    */
static idx_t cached_len = 0;          /* number of terms computed    */
static idx_t ptr8 = 1, ptr9 = 1;      /* pointers for next 8·value   */
static seq_t next8 = 0, next9 = 0;    /* next candidates              */

static void grow_and_generate(idx_t target)
{
    if (!cache) {                                   /* first call          */
        cache_cap = target + 1;                     /* allocate 1‑based    */
        cache = seq_alloc(cache_cap);
        if (!cache) return;                        /* allocation failed   */
        cache[1] = (seq_t)1;
        cached_len = 1;
        ptr8 = ptr9 = 1;
        next8 = cache[ptr8] * (seq_t)8;
        next9 = cache[ptr9] * (seq_t)9;
    }

    if (target <= cached_len) return;               /* nothing to do       */

    /* enlarge the cache if necessary */
    if (target + 1 > cache_cap) {
        seq_t *new_cache = seq_realloc(cache, cache_cap, target + 1);
        if (new_cache) {
            cache = new_cache;
            cache_cap = target + 1;
        } else {
            return; /* reallocation failed – keep previous data */
        }
    }

    /* generate missing terms */
    for (idx_t i = cached_len + 1; i <= target; ++i) {
        if (next8 < next9) {
            cache[i] = next8;
            ++ptr8;
            next8 = cache[ptr8] * (seq_t)8;
        } else if (next9 < next8) {
            cache[i] = next9;
            ++ptr9;
            next9 = cache[ptr9] * (seq_t)9;
        } else { /* equal */
            cache[i] = next8;                      /* or next9 */
            ++ptr8;
            next8 = cache[ptr8] * (seq_t)8;
            ++ptr9;
            next9 = cache[ptr9] * (seq_t)9;
        }
    }
    cached_len = target;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, but avoid UB */
    grow_and_generate(n);
    return cache[n];
}
