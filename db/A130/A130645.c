/*
 * A130645  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+439)^2 = y^2.
 * https://oeis.org/A130645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-3) - a(n-6) + 878  for n > 6
   Initial values (n=1..6):
     a(1)=0,  a(2)=44,  a(3)=1121,  a(4)=1317,
     a(5)=1541, a(6)=7644
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow out‑of‑order queries */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* array capacity (in elements) */
    static size_t cache_len = 0;   /* number of terms actually stored */

    /* initial sequence values */
    static const seq_t init[6] = {
        (seq_t)0,
        (seq_t)44,
        (seq_t)1121,
        (seq_t)1317,
        (seq_t)1541,
        (seq_t)7644
    };

    /* first call: allocate cache and fill the first six terms */
    if (cache == NULL) {
        cache_cap = 64;                      /* start with some capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
    }

    /* return if we already have the requested term */
    if (n <= cache_len) return cache[n - 1];

    /* enlarge array if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6] + (seq_t)878;
    }
    cache_len = n;

    return cache[n - 1];
}
