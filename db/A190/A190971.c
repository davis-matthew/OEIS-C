/*
 * A190971  a(n) = 5*a(n-1) - 10*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 5*a(n-1) – 10*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 19, i64 n <= 39, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* highest index already computed */
    static size_t cap      = 0;   /* allocated capacity */

    /* First call: allocate space for two initial terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 1;                        /* indices 0 and 1 are ready */
    }

    /* Ensure we have room for index n */
    if (n + 1 > cap) {
        size_t new_cap = (cap > 0 ? cap * 2 : 2);
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (computed < n) {
        idx_t next = computed + 1;
        cache[next] = (seq_t)5 * cache[computed]
                    - (seq_t)10 * cache[computed - 1];
        computed = next;
    }

    return cache[n];
}
