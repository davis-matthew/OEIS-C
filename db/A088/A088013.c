/*
 * A088013  Binomial transform of A001541 (with interpolated zeros).
 * https://oeis.org/A088013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((sqrt(2))^n + (-sqrt(2))^n + (2+sqrt(2))^n +
 *            (2-sqrt(2))^n)/4 Recurrence (derived from the generating
 *            function): a(0)=1, a(1)=1, a(2)=4, a(3)=10
 *            a(n)=4*a(n-1)-8*a(n-3)+4*a(n-4) for n≥4 This is an integer
 *            recurrence that works for both built‑in integer types and for
 *            the arbitrary‑precision...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;      /* cache of computed terms            */
static idx_t cache_size = 0;     /* number of terms currently stored   */

#ifdef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    /* Grow cache to hold up to n inclusive. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;

        if (cache == NULL) {
            /* First allocation. */
            new_cache = alloc_cache(new_size);
        } else {
            /* Reallocate: allocate new block, copy old elements, free old. */
            new_cache = alloc_cache(new_size);
            if (new_cache) {
                for (idx_t i = 0; i < cache_size; ++i) {
                    new_cache[i] = cache[i];
                }
                free_cache(cache);
            }
        }

        if (!new_cache) {
            /* Allocation failed – return zero. */
            return (seq_t)0;
        }

        cache = new_cache;
        cache_size = new_size;

        /* Compute terms up to n. */
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)4;
            } else if (i == 3) {
                cache[i] = (seq_t)10;
            } else {
                /* Recurrence: a(n)=4*a(n-1) - 8*a(n-3) + 4*a(n-4)  */
                seq_t t1 = cache[i - 1];
                t1 *= (seq_t)4;

                seq_t t2 = cache[i - 3];
                t2 *= (seq_t)8;

                seq_t t3 = cache[i - 4];
                t3 *= (seq_t)4;

                cache[i] = t1 - t2 + t3;
            }
        }
    }

    return cache[n];
}
