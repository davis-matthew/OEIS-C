/*
 * A223479  Rolling icosahedron face footprints: number of nX7 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves across an icosahedral edge.
 * https://oeis.org/A223479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 227*a(n-1) - 14764*a(n-2) + 411840*a(n-3)
 *                  - 5347200*a(n-4) + 29600000*a(n-5) - 48000000*a(n-6)
 *  for n > 6;  a(1) = 729, a(2) = 84375, a(3) = 11374425,
 *  a(4) = 1588785975, a(5) = 223894186425, a(6) = 31621425535575.
 *
 * This function memoises the sequence in a static array.  It works
 * for any idx_t>=1 and any seq_t that supports the required
 * arithmetic operators.  The implementation uses only plain C
 * arithmetics and no external helpers beyond <stdlib.h>.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n=0; return 0 as sentinel */

    /* Seed values for a(1)..a(6) */
    static const seq_t seed[6] = {
        (seq_t)729,
        (seq_t)84375,
        (seq_t)11374425,
        (seq_t)1588785975,
        (seq_t)223894186425,
        (seq_t)31621425535575
    };

    /* On first use, allocate cache and fill with seeds */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* number of terms currently stored */
    static size_t cap = 0;        /* allocated capacity */

    if (cache == NULL) {
        /* Determine an initial capacity (at least 6 terms) */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = seed[i];
        cached_n = 6;
    } else if (cached_n < 6) {
        /* If the cache was resized previously to <6 (unlikely), refill */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = seed[i];
        cached_n = 6;
    }

    /* Grow cache if needed */
    if ((idx_t)cap < n) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n */
    if (n > cached_n) {
        idx_t i;
        for (i = cached_n; i < n; ++i) {
            /* i is 0‑based: cache[i] will be a(i+1) */
            /* Use the recurrence (for i >= 6, i.e. n >= 7) */
            idx_t j = i; /* current index 0‑based = a(j+1) */
            seq_t val;
            if (j < 6) {          /* should not happen, handled earlier */
                val = seed[j];
            } else {
                /* compute recurrence using previous six terms */
                seq_t t1 = (seq_t)227          * cache[j - 1];
                seq_t t2 = (seq_t)14764        * cache[j - 2];
                seq_t t3 = (seq_t)411840       * cache[j - 3];
                seq_t t4 = (seq_t)5347200      * cache[j - 4];
                seq_t t5 = (seq_t)29600000     * cache[j - 5];
                seq_t t6 = (seq_t)48000000     * cache[j - 6];
                val =  t1 - t2 + t3 - t4 + t5 - t6;
            }
            cache[j] = val;
        }
        cached_n = n;
    }

    return cache[n - 1];
}
