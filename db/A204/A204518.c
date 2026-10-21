/*
 * A204518  Numbers such that floor(a(n)^2 / 6) is a square.
 * https://oeis.org/A204518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-3) - a(n-6),  n > 7 
   Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=10, a(7)=27. 
*/

#include <stdlib.h>
#include <stddef.h>

/* external linkage */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* cache[1..k] holds a(1)...a(k) */
    static idx_t  cached  = 0;      /* largest n for which cache is valid */
    static size_t cap     = 0;      /* allocated capacity (array size) */

    /* ensure cache exists and holds at least the first 7 terms */
    if (cap == 0) {
        /* initial capacity: 8 (index 0 unused) or bigger if n is big */
        cap = 8;
        if (cap < (size_t)(n + 1)) cap = (size_t)(n + 1);
        cache = (seq_t *)calloc(cap, sizeof(seq_t));   /* zero-initialised */
        if (!cache) abort();

        /* initialise first seven terms */
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 3;
        cache[5] = 5;
        cache[6] = 10;
        cache[7] = 27;
        cached = 7;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero‑initialise the new part */
        if (new_cap > cap) {
            size_t i;
            for (i = cap; i < new_cap; ++i) tmp[i] = 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    if (n > cached) {
        for (idx_t k = cached + 1; k <= n; ++k) {
            cache[k] = cache[k-3] * ((seq_t)10) - cache[k-6];
        }
        cached = n;
    }

    return cache[n];
}
