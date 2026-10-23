/*
 * A170407  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Formula used:  a(0)=1; for 1≤n≤43  : a(n)=2 + 12*(a(n-1)+…+a(0));
    for 44≤n      : a(n)=1 + 12*(a(n-1)+…+a(n-43)) – 78*a(n-44);
    (The recurrence follows from the rational generating function of this sequence.)  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed values      */
    static size_t  capacity = 0;     /* allocated size in elements   */
    static size_t  last_calculated = 0;  /* next index to compute        */

    /* allocate / enlarge cache as needed */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        capacity = need > 45 ? need : 45;
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (cache == NULL) abort();
    } else if (capacity < need) {
        size_t newcap = need * 2;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (newptr == NULL) abort();
        for (size_t i = capacity; i < newcap; ++i) newptr[i] = 0;
        cache = newptr;
        capacity = newcap;
    }

    /* compute values up to index n */
    for (size_t i = last_calculated; i < need; ++i) {
        idx_t idx = (idx_t)i;
        seq_t p = 0;
        if (idx == 0 || idx == 44)       p = (seq_t)1;
        else if (idx <= 43)              p = (seq_t)2;

        seq_t sum = 0;
        if (idx <= 43) {
            for (size_t j = 1; j <= idx; ++j)
                sum += cache[i - j];
            cache[i] = p + (sum * (seq_t)12);
        } else {
            for (size_t j = 1; j <= 43; ++j)
                sum += cache[i - j];
            cache[i] = p + (sum * (seq_t)12) - (cache[i - 44] * (seq_t)78);
        }
    }
    last_calculated = need;
    return cache[n];
}
