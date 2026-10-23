/*
 * A121956  a(n) = a(n-1) + (n-2)*a(n-2) + a(n-3) starting a(0)=0,
 *          a(1)=a(2)=1.
 * https://oeis.org/A121956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)+(n-2)*a(n-2)+a(n-3), a(0)=0, a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;
static idx_t cap   = 0;
static idx_t last  = (idx_t)-1;

/* Ensure that cache can hold indices up to new_cap-1.  The first call
   initializes base values a(0)=0, a(1)=1, a(2)=1.  Subsequent calls
   resize the array if necessary, preserving already computed terms. */
static void grow_cache(idx_t new_cap)
{
    if (cache == NULL) {
        cache = alloc_cache(new_cap);
        cap   = new_cap;
        cache[0] = 0;
        if (cap > 1) cache[1] = 1;
        if (cap > 2) cache[2] = 1;
        last = (cap > 2 ? 2 : (cap > 1 ? 1 : 0));
    } else if (new_cap > cap) {
        seq_t *new_arr = alloc_cache(new_cap);
        for (idx_t i = 0; i <= last; ++i) {
            new_arr[i] = cache[i];
        }
        if (new_cap > 1 && last < 1) new_arr[1] = 1;
        if (new_cap > 2 && last < 2) new_arr[2] = 1;
        free_cache(cache);
        cache = new_arr;
        cap   = new_cap;
        if (last < 2) last = (new_cap > 2 ? 2 : (new_cap > 1 ? 1 : 0));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    grow_cache(n + 1);

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = cache[i-1];
        term += (seq_t)(i - 2) * cache[i-2];
        term += cache[i-3];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
