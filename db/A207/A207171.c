/*
 * A207171  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(1)=6, a(2)=36, a(3)=90, a(4)=261, a(5)=624, a(6)=1482,
   a(7)=3808, a(8)=9512, a(9)=23280, a(10)=58080, a(11)=144996.
   For n>11:
   a(n) = a(n-1) + 9*a(n-3) + 2*a(n-4) + 2*a(n-5)
            - 12*a(n-6) - 8*a(n-7) + 8*a(n-9).          */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache and bookkeeping of computed values */
    static seq_t *cache = NULL;      /* cache[i] holds a(i), i>=0 */
    static size_t cache_size = 0;    /* number of allocated entries */
    static size_t computed   = 0;    /* highest index computed so far */
    static bool initialized  = false;

    /* First time: allocate space for indices 0..11 (11 inclusive) */
    if (!initialized) {
        cache_size = 12;            /* indices 0..11 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)                  /* allocation failure → 0 */
            return (seq_t)0;

        cache[0] = (seq_t)0;         /* unused */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)90;
        cache[4] = (seq_t)261;
        cache[5] = (seq_t)624;
        cache[6] = (seq_t)1482;
        cache[7] = (seq_t)3808;
        cache[8] = (seq_t)9512;
        cache[9] = (seq_t)23280;
        cache[10] = (seq_t)58080;
        cache[11] = (seq_t)144996;
        computed = 11;              /* highest index with a value */
        initialized = true;
    }

    /* Handle small n that are already cached */
    if (n <= (idx_t)computed)
        return cache[(size_t)n];

    /* Grow the cache so that index n is valid */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_size;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                  /* allocation failure → 0 */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)9   * cache[i - 3]
                 + (seq_t)2   * cache[i - 4]
                 + (seq_t)2   * cache[i - 5]
                 - (seq_t)12  * cache[i - 6]
                 - (seq_t)8   * cache[i - 7]
                 + (seq_t)8   * cache[i - 9];
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
