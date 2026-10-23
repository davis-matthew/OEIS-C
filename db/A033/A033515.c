/*
 * A033515  Number of matchings in graph C_{3} X P_{n}.
 * https://oeis.org/A033515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=6*a(n‑1)+9*a(n‑2)−a(n‑4);
   Initial values: a(0)=1, a(1)=4, a(2)=32, a(3)=228 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A static cache holds previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t last   = 3;    /* largest index already computed */
    static size_t cap   = 0;    /* allocated capacity (number of slots) */

    /* First call – allocate and initialise the cache. */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;       /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)228;
    }

    /* If the requested term is already cached, simply return it. */
    if (n <= last) return cache[n];

    /* Grow the cache as needed (doubling strategy). */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 4;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t val = 6 * cache[k - 1];
        val += 9 * cache[k - 2];
        val -= cache[k - 4];
        cache[k] = val;
    }

    last = n;
    return cache[n];
}
