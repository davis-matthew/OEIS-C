/*
 * A155602  a(n) = 4^n + 3^n - 1.
 * https://oeis.org/A155602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 3^n - 1 Recurrence used: a(0)=1, a(1)=6,
 *            a(n)=7*a(n-1)-12*a(n-2)-6 for n>1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;  /* array of computed terms */
static size_t cache_cap  = 0;     /* current capacity of cache */
static idx_t  computed_upto = 0;  /* largest index that has been filled */
static bool   base_initialised = false;  /* true once a(0) and a(1) are stored */

/* Ensure the cache can hold indices up to `n`.  If it is too small,
   resize it with realloc.  The new area is uninitialised; it will be
   filled by the recurrence when needed. */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* cast to seq_t* to satisfy C++ compiler */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* Make sure the cache is large enough */
    ensure_capacity(n);

    /* Initialise base values once */
    if (!base_initialised) {
        cache[0] = (seq_t)1;
        if (n >= 1) {
            cache[1] = (seq_t)6;
            computed_upto = 1;
        } else {
            computed_upto = 0;
        }
        base_initialised = true;
    } else if (computed_upto < 1 && n >= 1) {
        /* first call never supplied a(1) */
        cache[1] = (seq_t)6;
        computed_upto = 1;
    }

    /* Compute terms up to n using the recurrence */
    while (computed_upto < n) {
        idx_t i = computed_upto + 1;
        /* a(i) = 7*a(i-1) - 12*a(i-2) - 6 */
        seq_t val = (cache[computed_upto] * (seq_t)7)
                    - (cache[computed_upto - 1] * (seq_t)12)
                    - (seq_t)6;
        cache[i] = val;
        computed_upto = i;
    }

    return cache[n];
}
