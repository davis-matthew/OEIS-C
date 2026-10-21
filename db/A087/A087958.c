/*
 * A087958  a(n) is the square of the n-th partial sum minus the n-th partial
 *          sum of the squares, divided by a(n-1), for all n>=1, starting with
 *          a(0)=1, a(1)=5.
 * https://oeis.org/A087958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3), with a(0)=1, a(1)=5, a(2)=2,
 *            a(3)=17
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* dynamic array of computed terms */
    static idx_t stored = 0;      /* number of terms stored */
    static idx_t cap = 0;         /* allocated capacity */

    /* Initialisation of base terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) abort();        /* memory exhausted */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)17;
        stored = 4;
    }

    /* Extend cache if the requested index is beyond what we have */
    if ((idx_t)n >= stored) {
        /* Resize to at least n+1 */
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= (idx_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();          /* memory exhausted */
        cache = tmp;
        cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (idx_t i = stored; i <= (idx_t)n; ++i) {
            seq_t term = cache[i-1] + cache[i-2] * (seq_t)3;
            term -= cache[i-3];
            cache[i] = term;
        }
        stored = (idx_t)n + 1;
    }

    return cache[n];
}
