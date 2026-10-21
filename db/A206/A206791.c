/*
 * A206791  Number of n X 2 0..3 arrays avoiding the pattern z z+1 z in any
 *          row, column, diagonal or antidiagonal.
 * https://oeis.org/A206791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 11*a(n-2) + 51*a(n-3) + 13*a(n-4) - 3*a(n-5)
 *            - a(n-6); Initial values: a(1)=16, a(2)=256, a(3)=3721,
 *            a(4)=53824, a(5)=779689, a(6)=11296321 (empirical recurrence;
 *            holds for all n≥7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */

    /* Static cache: a_cache[i] holds a(i+1) */
    static seq_t *a_cache = NULL;
    static idx_t cache_len = 0;                /* number of computed terms */
    static idx_t cache_cap = 0;                /* allocated capacity */

    /* initialise first 6 terms on first call */
    if (cache_len == 0) {
        cache_cap = 16;                       /* initial capacity */
        a_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!a_cache) abort();                /* allocation failure */

        a_cache[0] = 16;      /* a(1) */
        a_cache[1] = 256;     /* a(2) */
        a_cache[2] = 3721;    /* a(3) */
        a_cache[3] = 53824;   /* a(4) */
        a_cache[4] = 779689;  /* a(5) */
        a_cache[5] = 11296321;/* a(6) */
        cache_len = 6;
    }

    /* if requested index is beyond allocated capacity, enlarge */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();     /* allocation failure */
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_len) {
        for (idx_t i = cache_len; i < n; ++i) {
            /* recurrence uses indices i-1 .. i-6 (valid because i >= 6) */
            a_cache[i] = 15 * a_cache[i-1]
                        - 11 * a_cache[i-2]
                        + 51 * a_cache[i-3]
                        + 13 * a_cache[i-4]
                        -  3 * a_cache[i-5]
                        -  a_cache[i-6];
        }
        cache_len = n;
    }

    return a_cache[n-1];
}
