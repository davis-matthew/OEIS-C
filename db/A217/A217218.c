/*
 * A217218  Trajectory of 44 under the map k -> A006368(k).
 * https://oeis.org/A217218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   A006368_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A006368_func.h"

/* Recurrence: a(1)=44; a(n+1)=f(a(n))
   where f(k)=k + k/2 for even k;
   f(k)=k - k/4 for odd k with k%4==1;
   f(k)=k - (k+3)/4 for odd k with k%4==3. */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;                     /* undefined for offset 1 */
    }

    /* Ensure the cache can hold n elements. */
    if ((size_t)n > cache_len) {
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < (size_t)n) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {
                cache[i] = 44;                       /* a(1) */
            } else {
                cache[i] = A006368_func(cache[i-1]); /* a(i+1) */
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
