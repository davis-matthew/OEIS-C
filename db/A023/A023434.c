/*
 * A023434  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-4).
 * https://oeis.org/A023434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 78, u64 n <= 156,
 *            u128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) – a(n-4)  (n > 3)
   Initial values: a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2   */
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *a_cache = NULL;      /* array of computed terms          */
static idx_t a_cache_cap = 0;      /* allocated size of a_cache        */
static idx_t a_cache_max = 0;      /* highest index that has a value   */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;

    /* Ensure the cache covers indices 0..n */
    if (n > a_cache_max) {
        /* Grow capacity if necessary */
        if (a_cache_cap == 0) {
            a_cache_cap = 4;            /* at least enough for 0..3 */
        }
        while (a_cache_cap <= n) {
            a_cache_cap <<= 1;          /* double capacity */
            if (a_cache_cap == 0) {     /* overflow guard */
                a_cache_cap = n + 1;
                break;
            }
        }

        seq_t *tmp = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failed */
        a_cache = tmp;

        /* Initialise base values if the cache was empty */
        if (a_cache_max < 3) {
            a_cache[0] = 0;
            a_cache[1] = 1;
            a_cache[2] = 1;
            a_cache[3] = 2;
            a_cache_max = 3;
        }

        /* Compute terms up to n */
        for (idx_t i = a_cache_max + 1; i <= n; ++i) {
            a_cache[i] = a_cache[i - 1] + a_cache[i - 2] - a_cache[i - 4];
        }
        a_cache_max = n; /* Update highest computed index */
    }

    return a_cache[n];
}
