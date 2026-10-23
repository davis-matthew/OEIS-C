/*
 * A188183  Number of strictly increasing arrangements of 5 numbers in
 *          -(n+3)..(n+3) with sum zero.
 * https://oeis.org/A188183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11) * a(1) = 12, a(2) = 32, a(3) = 73, a(4) =
 *            141, a(5) = 252, a(6) = 414, * a(7) = 649, a(8) = 967, a(9) =
 *            1394, a(10) = 1944, a(11) = 2649
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Type definitions supplied by the build system:
 *  typedef ... seq_t;
 *  typedef uint64_t idx_t;
 */

static const seq_t init_vals[25] = {
    0,
    12, 32, 73, 141, 252, 414, 649, 967,
    1394, 1944, 2649, 3523, 4604, 5910, 7483, 9343,
    11538, 14090, 17053, 20451, 24342, 28754, 33751, 39361
};

static seq_t *cache = NULL;           /* stored terms, cache[0] unused            */
static idx_t  cache_size = 0;         /* number of allocated elements             */
static idx_t  computed_up_to = 0;     /* highest index for which value is valid   */
static int    init_done = 0;          /* flag for one‑time initialisation          */

static void grow_cache(idx_t new_size)
{
    /* Allocate a new array large enough to hold indices up to new_size-1. */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_size];
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
    cache = new_cache;
    cache_size = new_size;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp) {
        cache = tmp;
        cache_size = new_size;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, return 0 as sentinel */

    /* Ensure the cache can hold indices up to n. */
    if (cache_size <= n) {
        idx_t new_cap = cache_size ? cache_size * 2 : 256;
        if (new_cap <= n) new_cap = n + 1;
        grow_cache(new_cap);
    }

    /* First time: copy the known initial terms into the cache. */
    if (!init_done) {
        for (idx_t i = 1; i <= 24 && i < cache_size; ++i)
            cache[i] = init_vals[i];
        init_done = 1;
        computed_up_to = (idx_t)24;
    }

    /* Compute any missing values up to n using the recurrence. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i < 12)   /* initial terms already available */
            continue;

        seq_t val = cache[i - 1];
        val = val + val;                   /* 2*a(i-1) */
        val = val - cache[i - 3];          /* -a(i-3) */
        val = val - cache[i - 5];          /* -a(i-5) */
        val = val - cache[i - 5];          /* -a(i-5) again */
        val = val + cache[i - 6];          /* +a(i-6) */
        val = val + cache[i - 6];          /* +a(i-6) again */
        val = val + cache[i - 8];          /* +a(i-8) */
        val = val - cache[i -10];          /* -a(i-10) */
        val = val - cache[i -10];          /* -a(i-10) again */
        val = val + cache[i -11];          /* +a(i-11) */
        cache[i] = val;
    }
    if (n > computed_up_to)
        computed_up_to = n;

    return cache[n];
}
