/*
 * A062147  Row sums of unsigned triangle A062137 (generalized a=3 Laguerre).
 * https://oeis.org/A062147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+3)*a(n-1) - (n-1)*(n+2)*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values of the sequence      */
    static size_t cap    = 0;      /* allocated capacity                 */
    static idx_t  computed = 0;    /* number of values computed so far   */

    /* handle the two known base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* first allocation – store the two base values */
    if (cap == 0) {
        cap      = 2;
        cache    = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        computed = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                /* out‑of‑memory fallback */
        cache = tmp;
        cap   = newcap;
    }

    /* compute any missing values up to n */
    while (computed <= n) {
        idx_t i = computed;
        /* recurrence: a(i) = (2*i+3)*a(i-1) - (i-1)*(i+2)*a(i-2) */
        seq_t term = ((seq_t)(2*i + 3) * cache[i-1])
                     - ((seq_t)((i-1)*(i+2)) * cache[i-2]);
        cache[i] = term;
        computed++;
    }

    /* return the requested term */
    return cache[n];
}
