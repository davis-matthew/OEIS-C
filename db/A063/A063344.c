/*
 * A063344  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          71 ).
 * https://oeis.org/A063344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13.
   Base values (offset 2):
   a(2)= -1, a(3)= 176, a(4)= 385, a(5)= 595, a(6)= 805,
   a(7)=1015, a(8)=1225, a(9)=1435, a(10)=1645, a(11)=1855,
   a(12)=2065, a(13)=2273.
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size of array */
static idx_t cache_max = 1;       /* highest index computed so far (inclusive) */

/* Ensure the cache can hold indices up to n.  */
static void
ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialize base terms up to index 13.  */
static void
init_base(void)
{
    static const seq_t base[14] = {
        0,            /* a(0)   unused, but set to 0 */
        0,            /* a(1)   unused */
        -1,           /* a(2) */
         176,         /* a(3) */
         385,         /* a(4) */
         595,         /* a(5) */
         805,         /* a(6) */
        1015,         /* a(7) */
        1225,         /* a(8) */
        1435,         /* a(9) */
        1645,         /* a(10) */
        1855,         /* a(11) */
        2065,         /* a(12) */
        2273          /* a(13) */
    };
    ensure_cache(13);
    for (idx_t i = 0; i <= 13; ++i)
        cache[i] = base[i];
    cache_max = 13;
}

/* Public function required by the system. */
seq_t a(idx_t n)
{
    if (n < 2) /* offset not defined; return 0 */
        return (seq_t)0;

    /* Initialize cache on first call. */
    if (!cache) {
        init_base();
    } else if (cache_max < 13) {      /* not yet initialized base */
        init_base();
    }

    if (n > cache_max) {
        /* Compute missing values up to n. */
        ensure_cache(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 13) {
                /* values already set by init_base */
                continue;
            }
            /* recurrence a(i)=a(i-4)+a(i-6)-a(i-10) */
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        cache_max = n;
    }
    return cache[n];
}
