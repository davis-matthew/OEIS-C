/*
 * A204636  Number of (n+2)X4 0..3 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally exactly three ways, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A204636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204636
   Empirical linear recurrence for n > 13:
   a(n) = 4*a(n-1)+a(n-2)-2*a(n-3)-19*a(n-4)-4*a(n-5)
          +21*a(n-6)+24*a(n-7)+3*a(n-8)-18*a(n-9)-9*a(n-10)
   Initial terms:
   a(1)=405, a(2)=1884, a(3)=5974, a(4)=379, a(5)=1860,
   a(6)=2239, a(7)=8140, a(8)=32206, a(9)=107738, a(10)=407469,
   a(11)=1548886, a(12)=5829163, a(13)=22096946
*/

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
# include <new>
#endif

/* cached values, 1‑indexed; cache[0] is unused */
static seq_t *cache = NULL;
/* capacity of the cache array (including index 0) */
static idx_t cache_cap = 0;
/* highest index that has been computed */
static idx_t last_computed = 0;

/* ensure the cache is large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t old_cap = cache_cap;
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;

#ifdef __cplusplus
        /* construct the new elements (trivial for arithmetic types) */
        for (idx_t i = old_cap; i < cache_cap; ++i) {
            new (&cache[i]) seq_t();
        }
#endif
    }
}

/* initialise the first 13 terms once */
static void init_initial_terms(void)
{
    if (last_computed >= 13) return; /* already initialised */
    ensure_cache(13);
    cache[1]  = 405;
    cache[2]  = 1884;
    cache[3]  = 5974;
    cache[4]  = 379;
    cache[5]  = 1860;
    cache[6]  = 2239;
    cache[7]  = 8140;
    cache[8]  = 32206;
    cache[9]  = 107738;
    cache[10] = 407469;
    cache[11] = 1548886;
    cache[12] = 5829163;
    cache[13] = 22096946;
    last_computed = 13;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined below offset */

    ensure_cache(n);
    init_initial_terms();

    if (n <= last_computed) return cache[n];

    /* compute missing values up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t v = 4 * cache[i-1]  + cache[i-2]
                  - 2 * cache[i-3]  - 19 * cache[i-4]
                  - 4 * cache[i-5]  + 21 * cache[i-6]
                  + 24 * cache[i-7] + 3 * cache[i-8]
                  - 18 * cache[i-9] - 9 * cache[i-10];
        cache[i] = v;
    }
    last_computed = n;
    return cache[n];
}
