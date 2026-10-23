/*
 * A020879  Number of elementary edge-subgraphs in Moebius ladder M_n.
 * https://oeis.org/A020879
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 11*a(n-3) - 8*a(n-4) + a(n-5) +
 *            a(n-6) with offset 2 and initial values: a(2)=17, a(3)=58,
 *            a(4)=181, a(5)=602, a(6)=2006, a(7)=6797 (for n>7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of a values starting at n=2   */
static size_t cache_cap = 0;     /* allocated capacity                 */
static size_t cache_size = 0;    /* number of values stored (equals
                                   number of indices from n=2)        */

/* initialise cache with the first six values (n=2..7). */
static void init_cache(void)
{
    size_t cap = 16;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) return;          /* allocation failure: caller will handle */
    cache_cap = cap;
    cache[0] = 17;   /* a(2) */
    cache[1] = 58;   /* a(3) */
    cache[2] = 181;  /* a(4) */
    cache[3] = 602;  /* a(5) */
    cache[4] = 2006; /* a(6) */
    cache[5] = 6797; /* a(7) */
    cache_size = 6;
}

/* ensure that cache has enough space for at least up_to indices */
static void grow_cache(size_t up_to)
{
    if (cache_cap >= up_to) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < up_to) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
    /* if realloc fails we simply keep the old capacity; the following
       loop will not try to write beyond the allocated space. */
}

/* main function required by the build system */
seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* offset is 2; behaviour below offset is irrelevant */

    size_t idx = (size_t)n - 2;   /* index in our cache array */

    if (cache == NULL) init_cache();
    if (cache == NULL) return 0;  /* allocation failure */

    /* extend cache if necessary */
    if (idx >= cache_size) {
        grow_cache(idx + 1);
        /* compute missing terms up to idx */
        for (size_t j = cache_size; j <= idx; ++j) {
            /* j corresponds to n = j + 2 */
            if (j < 6) {
                /* values for n=2..7 are already initialised */
                continue;
            }
            /* recurrence: compute

               a = 4*a(n-1) + 2*a(n-2) - 11*a(n-3) - 8*a(n-4) + a(n-5) + a(n-6)

               where indices in cache are offset by 2. */
            seq_t pos = cache[j - 1] * (seq_t)4;
            pos += cache[j - 2] * (seq_t)2;
            pos += cache[j - 5];
            pos += cache[j - 6];

            seq_t neg = cache[j - 3] * (seq_t)11;
            neg += cache[j - 4] * (seq_t)8;

            /* pos is guaranteed to be >= neg for all n>=8, so subtraction
               cannot underflow even for unsigned types. */
            cache[j] = pos - neg;
        }
        cache_size = idx + 1;
    }
    return cache[idx];
}
