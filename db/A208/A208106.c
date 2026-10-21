/*
 * A208106  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 17*a(n-2) - 124*a(n-3) + 276*a(n-4) +
 *            396*a(n-5) - 902*a(n-6) - 462*a(n-7) + 946*a(n-8) + 220*a(n-9) -
 *            340*a(n-10) - 44*a(n-11) + 39*a(n-12) + 3*a(n-13) - a(n-14) for
 *            n > 15 Initial values: a(1)=25, a(2)=625, a(3)=2209, a(4)=9025,
 *            a(5)=46225, a(6)=261121, a(7)=1600225, a(8)=102...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: index 0 is unused so that a(k) is cache[k] for k>=1 */
    static seq_t *cache = NULL;          /* pointer to cache array */
    static idx_t cache_size = 0;         /* allocated elements count */
    static idx_t max_computed = 0;       /* highest n for which cache[n] is valid */

    /* Initial 15 values.  Index 0 is left 0 for convenience. */
    static const seq_t init_vals[16] = {
        0ULL,                      /* unused index 0 */
        25ULL,                     /* a(1) */
        625ULL,                    /* a(2) */
        2209ULL,                   /* a(3) */
        9025ULL,                   /* a(4) */
        46225ULL,                  /* a(5) */
        261121ULL,                 /* a(6) */
        1600225ULL,                /* a(7) */
        10220809ULL,               /* a(8) */
        67158025ULL,               /* a(9) */
        448380625ULL,              /* a(10) */
        3024450025ULL,             /* a(11) */
        20525433289ULL,            /* a(12) */
        139828879969ULL,           /* a(13) */
        954796716769ULL,           /* a(14) */
        6529082812849ULL           /* a(15) */
    };

    /* Initialise cache on the first call. */
    if (!cache)
    {
        cache_size = 16;                      /* need indices 0..15 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure → 0 */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
        max_computed = 15;
    }

    /* Return quickly if the requested term is already available. */
    if (n <= max_computed)
        return cache[n];

    /* Resize cache if necessary to hold indices up to n. */
    if (n + 1 > cache_size)
    {
        idx_t new_size = cache_size * 2;
        if (n + 1 > new_size)
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
        {
            free(cache);
            return (seq_t)0;                 /* allocation failure → 0 */
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        if (k <= 15)          /* safety: values 1..15 are already known */
            continue;

        /* Positive contributions */
        seq_t pos = 0;
        pos += 11ULL * cache[k - 1];
        pos += 276ULL * cache[k - 4];
        pos += 396ULL * cache[k - 5];
        pos += 946ULL * cache[k - 8];
        pos += 220ULL * cache[k - 9];
        pos += 39ULL  * cache[k - 12];
        pos += 3ULL   * cache[k - 13];

        /* Negative contributions */
        seq_t neg = 0;
        neg += 17ULL  * cache[k - 2];
        neg += 124ULL * cache[k - 3];
        neg += 902ULL * cache[k - 6];
        neg += 462ULL * cache[k - 7];
        neg += 340ULL * cache[k - 10];
        neg += 44ULL  * cache[k - 11];
        neg += cache[k - 14];            /* coefficient 1 */

        cache[k] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}
