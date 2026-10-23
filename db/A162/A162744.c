/*
 * A162744  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=7, a(2)=42, a(3)=231;
   for n ≥ 4: a(n) = 5*a(n‑1) + 5*a(n‑2) – 15*a(n‑3)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cap   = 0;       /* allocated capacity */
    static idx_t  last  = 0;       /* highest index computed */

    /* initialize the first four terms on first call */
    if (cache == NULL) {
        cap  = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)231;
        last = 3;
    }

    /* Ensure the array is large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* If we already have the term, return it */
    if (n <= last) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 3) {
            /* already set at initialization – should not happen */
            continue;
        } else {
            cache[i] = (seq_t)5 * cache[i-1] + (seq_t)5 * cache[i-2]
                       - (seq_t)15 * cache[i-3];
        }
    }
    last = n;
    return cache[n];
}
