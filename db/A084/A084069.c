/*
 * A084069  Numbers k such that 7*k^2 = floor(k*sqrt(7)*ceiling(k*sqrt(7))).
 * https://oeis.org/A084069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A084069:  Numbers k such that 7*k^2 = floor(k*sqrt(7)*ceil(k*sqrt(7))).
 *
 * The sequence satisfies the linear recurrence
 *    a(1)=1, a(2)=3, a(3)=17, a(4)=48,
 *    a(n) = 16*a(n-2) – a(n-4)   for n > 4.
 *
 * The implementation caches already computed terms so that calls for
 * arbitrary n (in any order) are answered in O(n) time with minimal
 * overhead.  For 64‑bit seq_t values the multiplication is performed in
 * __int128 to avoid overflow during the intermediate step.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache, 1‑based */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;  /* cache capacity (index‑based, cache[0] unused) */
    static idx_t last  = 0;  /* highest index that has been computed */

    if (n == 0)            /* offset is 1; n < 1 never requested by the framework */
        return (seq_t)0;

    /* ensure the cache is large enough for index n */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 128;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first four terms once */
    if (last < 1) { cache[1] = (seq_t)1; last = 1; }
    if (last < 2) { cache[2] = (seq_t)3; last = 2; }
    if (last < 3) { cache[3] = (seq_t)17; last = 3; }
    if (last < 4) { cache[4] = (seq_t)48; last = 4; }

    /* compute up to n using the 4‑term recurrence */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* use 128‑bit intermediate for 64‑bit types */
                __int128 t = (__int128)cache[i - 2] * 16 - (__int128)cache[i - 4];
                cache[i] = (seq_t)t;
            } else {
                /* arbitrary‑precision type handles its own overflow */
                cache[i] = (seq_t)(16 * cache[i - 2] - cache[i - 4]);
            }
        }
        last = n;
    }

    return cache[n];
}
