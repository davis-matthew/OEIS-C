/*
 * A030032  a(n+1) = Sum_{k=0..floor(n/3)} a(k) * a(n-k).
 * https://oeis.org/A030032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 34, u64 n <= 62, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Recurrence: a(0)=1; for n>=0, a(n+1)= Σ_{k=0..⌊n/3⌋} a(k)*a(n-k) */

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache if necessary */
    if (!cache) {
        cache_cap = (size_t)(n + 1 > 512 ? n + 1 : 512);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* Allocation failure – not expected in tests */
    } else if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            idx_t limit = (i - 1) / 3;            /* floor((i-1)/3) */
            seq_t sum = 0;
            for (idx_t k = 0; k <= limit; ++k) {
                sum += cache[k] * cache[(i - 1) - k];
            }
            cache[i] = sum;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
