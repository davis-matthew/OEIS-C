/*
 * A042203  Denominators of continued fraction convergents to sqrt(627).
 * https://oeis.org/A042203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1252*a(n-2) - a(n-4), a(0)=1, a(1)=25, a(2)=1251,
 *            a(3)=31300
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A042203.
 * The sequence is defined by: a(0)=1, a(1)=25, a(2)=1251, a(3)=31300,
 * and for n>3: a(n)=1252*a(n-2)-a(n-4). */
seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size of cache */
    static size_t len   = 0;    /* number of terms stored */

    /* ---------- handle the first four terms directly ---------- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)1251;
    if (n == 3) return (seq_t)31300;

    /* ---------- initialise cache if first time ---------- */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* initialise first four values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)1251;
        cache[3] = (seq_t)31300;
        len = cap;
    }

    /* ---------- extend cache to required size ---------- */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – in practice this should never happen */
            abort();
        }
        cache = tmp;
        cap = new_cap;
        /* compute missing terms up to n */
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = (seq_t)1252 * cache[i-2] - cache[i-4];
        }
        len = cap;
    }
    /* ---------- cache already contains term ---------- */
    return cache[n];
}
