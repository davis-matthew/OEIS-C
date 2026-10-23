/*
 * A041677  Denominators of continued fraction convergents to sqrt(357).
 * https://oeis.org/A041677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6802*a(n-6) - a(n-12) for n>11 initial values: a(0)=1,
 *            a(1)=1, a(2)=9, a(3)=19, a(4)=161, a(5)=180, a(6)=6641,
 *            a(7)=6821, a(8)=61209, a(9)=129239, a(10)=1095121, a(11)=1224360
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Number of terms we keep in the static cache.
   The sequence grows fast; 20000 is far beyond the 10000‑term bound in the
   specification and consumes only a few megabytes even with 128‑bit ints. */
#define MAX_CACHE 20000

static const seq_t init[12] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)9,
    (seq_t)19,
    (seq_t)161,
    (seq_t)180,
    (seq_t)6641,
    (seq_t)6821,
    (seq_t)61209,
    (seq_t)129239,
    (seq_t)1095121,
    (seq_t)1224360
};

/* Static cache */

static seq_t cache[MAX_CACHE];
static size_t cache_size = 0;
static int cache_initialized = 0;

/*------------------------------------------------------------*/

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* If the requested index is beyond the static cache, compute
       directly in a temporary array.  This path is never hit for
       the problem limits (n ≤ about 10000). */
    if (idx >= MAX_CACHE) {
        seq_t *tmp = (seq_t*)malloc((idx + 1) * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = 0; i < 12 && i <= idx; ++i) tmp[i] = init[i];
        for (size_t i = 12; i <= idx; ++i)
            tmp[i] = (seq_t)6802 * tmp[i-6] - tmp[i-12];
        seq_t result = tmp[idx];
        free(tmp);
        return result;
    }

    /* Lazy static cache initialization */
    if (!cache_initialized) {
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_size = 12;
        cache_initialized = 1;
    }

    /* Return cached value if already computed */
    if (idx < cache_size)
        return cache[idx];

    /* Compute missing terms up to idx */
    for (size_t i = cache_size; i <= idx; ++i)
        cache[i] = (seq_t)6802 * cache[i-6] - cache[i-12];

    cache_size = idx + 1;
    return cache[idx];
}
