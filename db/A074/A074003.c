/*
 * A074003  Number of elements of GF(3^n) with trace 1 and subtrace 0.
 * https://oeis.org/A074003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 27*a(n-5) + 18*a(n-4) + 9*a(n-3) + 3*a(n-2)  for n >= 6
 * Initial values: a(1)=1, a(2)=0, a(3)=3, a(4)=9, a(5)=30
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t size = 0; /* number of elements in cache */

    if (n == 0) {
        /* undefined for n < 1, but return 0 to avoid UB */
        return (seq_t)0;
    }

    size_t idx = (size_t)(n - 1); /* zero‑based index */

    /* grow cache if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure */
            return (seq_t)0;
        }
        /* initialize the first five terms */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)9;   /* a(4) */
        cache[4] = (seq_t)30;  /* a(5) */
        size = 5;
    }

    if (cache != NULL && idx >= size) {
        /* enlarge cache to accommodate idx+1 elements */
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0; /* allocation failure */
        }
        cache = tmp;
        /* compute new elements in order */
        for (size_t i = size; i <= idx; ++i) {
            /* i is zero‑based, corresponds to a(i+1) */
            seq_t v2 = cache[i - 1];       /* a(i)   */
            seq_t v3 = cache[i - 2];       /* a(i-1) */
            seq_t v4 = cache[i - 3];       /* a(i-2) */
            seq_t v5 = cache[i - 4];       /* a(i-3) */
            seq_t v6 = cache[i - 5];       /* a(i-4) */
            cache[i] = (seq_t)27 * v6
                     + (seq_t)18 * v5
                     + (seq_t) 9 * v4
                     + (seq_t) 3 * v3;
        }
        size = new_size;
    }

    return cache[idx];
}
