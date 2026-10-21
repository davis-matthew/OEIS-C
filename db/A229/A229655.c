/*
 * A229655  Quintisection a(5n+k) gives k-th differences of a for k=0..4 with
 *          a(n)=0 for n<4 and a(4)=1.
 * https://oeis.org/A229655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, eigen
 * Formula:   a(5*n) = a(n) a(5*n+1) = a(n+1) - a(n) a(5*n+2) = a(n+2) -
 *            2*a(n+1) + a(n) a(5*n+3) = a(n+3) - 3*a(n+2) + 3*a(n+1) - a(n)
 *            a(5*n+4) = a(n+4) - 4*a(n+3) + 6*a(n+2) - 4*a(n+1) + a(n)
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1
 * Range:     i8 n <= 158, i16 n <= 7808, i32 n <= 9205, i64 n <= 9205,
 *            i128 n <= 9205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <new>
#endif

/* cache for already computed terms */
static seq_t *cache = NULL;          /* pointer to the array */
static size_t capacity = 0;          /* allocated capacity */
static size_t len = 0;               /* number of terms already computed */

/* helper to initialise an entry to 0, respecting C and C++ */
static inline void init_entry(seq_t *p, size_t i)
{
#if defined(__cplusplus)
    /* placement new constructs a default object */
    new (p + i) seq_t();
#else
    *p = 0;
#endif
}

/* prepare at least up to new_cap elements, computing missing ones */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= capacity)
        return;                                 /* nothing to do */
    if (!cache) {                               /* first time allocation */
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();
        capacity = new_cap;
        len = 0;
    } else {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }
    /* compute from len up to new_cap-1 */
    for (size_t i = len; i < new_cap; ++i) {
        if (i < 5) {                            /* base values */
            if (i == 4)
                init_entry(cache, i);           /* cache[4] = 1; set below */
            else
                init_entry(cache, i);           /* cache[0..3] = 0 */
        } else {
            idx_t q = (idx_t)i / 5;
            uint8_t r = (uint8_t)(i % 5);
            seq_t val;
            switch (r) {
                case 0:
                    val = cache[q];
                    break;
                case 1:
                    val = cache[q + 1] - cache[q];
                    break;
                case 2:
                    val = cache[q + 2] - cache[q + 1] * 2 + cache[q];
                    break;
                case 3:
                    val = cache[q + 3] - cache[q + 2] * 3
                          + cache[q + 1] * 3 - cache[q];
                    break;
                default: /* r == 4 */
                    val = cache[q + 4] - cache[q + 3] * 4
                          + cache[q + 2] * 6
                          - cache[q + 1] * 4 + cache[q];
                    break;
            }
            init_entry(cache, i);
            cache[i] = val;
        }
    }
    len = new_cap;
}

/* Public function required by the task */
seq_t a(idx_t n)
{
    /* ensure base values are initialised up to at least 5 entries */
    if (capacity < 5) {
        ensure_capacity(5);
        /* set known initial constants */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
    }
    if ((size_t)n >= len) {
        ensure_capacity((size_t)n + 1);
    }
    return cache[n];
}
