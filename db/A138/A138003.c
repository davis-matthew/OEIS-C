/*
 * A138003  Binomial transform of 1, 1, 0, -1, -1 (periodically continued).
 * https://oeis.org/A138003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=3, a(4)=0; for n>=5:
 *            a(n)=3*a(n-1)-4*a(n-2)+2*a(n-3)-a(n-4)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 45, i64 n <= 90, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of computed terms */

    size_t idx = (size_t)n;       /* index accessed */

    /* Ensure sufficient capacity */
    if (idx >= cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the first five terms once */
    if (size < 5) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)0;
        size = 5;
    }

    /* Compute any missing terms up to idx */
    for (size_t i = size; i <= idx; ++i) {
        cache[i] = 3 * cache[i-1] - 4 * cache[i-2]
                 + 2 * cache[i-3] - cache[i-4];
    }
    size = idx + 1;

    return cache[idx];
}
