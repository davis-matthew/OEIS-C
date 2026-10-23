/*
 * A135435  a(n) = a(n-4) + a(n-7) with a(0), ..., a(6) = [7,0,0,0,4,0,0].
 * https://oeis.org/A135435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 84, u32 n <= 171, u64 n <= 342,
 *            u128 n <= 685, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135435 – a(n) = a(n-4) + a(n-7),
   with a(0)=7, a(1)=0, a(2)=0, a(3)=0, a(4)=4, a(5)=0, a(6)=0   */
#include <stdlib.h>
#include <string.h>

static seq_t *cache      = NULL;   /* cached values                     */
static idx_t cache_len   = 0;      /* number of allocated elements      */
static idx_t last_done   = (idx_t)-1;  /* last index for which cache[i] is valid */

/* make sure cache has room for indices up to n and that the base
   values a(0)..a(6) are initialised. */
static void ensure_cache(idx_t n)
{
    /* we always need at least indices 0..6 for the recurrence      */
    idx_t need = n + 1;
    if (need < 7) need = 7;               /* at least 7 elements */

    if (cache_len < need) {                /* enlarge buffer if required */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
        if (!tmp) {                        /* allocation failure – abort */
            abort();
        }
        cache     = tmp;
        cache_len = need;
    }

    /* initial values only once                         */
    if (last_done == (idx_t)-1) {
        static const seq_t init[7] = {7, 0, 0, 0, 4, 0, 0};
        memcpy(cache, init, sizeof(init));
        last_done = 6;                     /* indices 0..6 are now valid*/
    }

    /* extend the cache up to n using the recurrence          */
    if (last_done < n) {
        for (idx_t i = last_done + 1; i <= n; ++i) {
            cache[i] = cache[i - 4] + cache[i - 7];
        }
        last_done = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
