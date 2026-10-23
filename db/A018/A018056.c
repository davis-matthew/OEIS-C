/*
 * A018056  Expansion of g.f. 1/((1-3*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A018056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 123*a(n-2) + 216*a(n-3) for n>2; a(0)=1,
 *            a(1)=20, a(2)=277
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Initialise cache on first use */
    if (cache == NULL) {
        cap = 4;                                      /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;     /* a(0) */
        cache[1] = (seq_t)20;    /* a(1) */
        cache[2] = (seq_t)277;   /* a(2) */
        len   = 3;
    }

    /* Grow cache if we need to store more terms */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms via the linear recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1;        continue; }
        if (i == 1) { cache[1] = (seq_t)20;       continue; }
        if (i == 2) { cache[2] = (seq_t)277;      continue; }

        seq_t term = (seq_t)20 * cache[i-1]
                   + (seq_t)216 * cache[i-3]
                   - (seq_t)123 * cache[i-2];
        cache[i] = term;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
