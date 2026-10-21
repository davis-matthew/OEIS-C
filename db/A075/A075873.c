/*
 * A075873  40*n^2 + 9 is a square.
 * https://oeis.org/A075873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075873: a(n) = 38*a(n-3) - a(n-6)
   with initial values a(1)=0,  a(2)=1,  a(3)=2,
                          a(4)=9,  a(5)=40, a(6)=77 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..6 */
    static const seq_t base[6] = {0,1,2,9,40,77};

    /* Static cache for values that have been computed so far */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored in cache */

    /* Return base value directly if requested */
    if (n <= 6)
        return base[n-1];

    /* Ensure the cache has enough space to hold all needed terms */
    size_t need = (size_t)n;
    if (cap < need) {
        size_t new_cap = (cap == 0) ? 16 : cap;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Fill the cache up to index n (1‑based). */
    while (len < need) {
        if (len < 6) {            /* fill base terms */
            cache[len] = base[len];
            len++;
            continue;
        }
        /* Recurrence: a(k) = 38*a(k-3) - a(k-6)  (k = len+1) */
        seq_t val = cache[len-3] * (seq_t)38;
        val -= cache[len-6];
        cache[len] = val;
        len++;
    }

    /* Return the requested term (0‑based index = n-1). */
    return cache[n-1];
}
