/*
 * A041901  Denominators of continued fraction convergents to sqrt(472).
 * https://oeis.org/A041901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 613834*a(n-14) - a(n-28) * Initial values: * a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=11, * a(6)=51, a(7)=266,
 *            a(8)=1115, a(9)=1381, a(10)=2496, * a(11)=3877, a(12)=10250,
 *            a(13)=14127, a(14)=603584, * a(15)=617711, a(16)=1839006,
 *            a(17)=2456717, a(18)=4295723, * a(19)=6752440, a(20)=313...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t   cache_size = 0;   /* number of terms already stored */
static size_t  cache_cap  = 0;   /* allocated capacity */

/* coefficients of the continued fraction for sqrt(472),
   after the initial term a0=21.  The period is 14. */
static const int cf_coeff[14] = {
    1, 2, 1, 1, 1, 4, 5, 4, 1, 1, 1, 2, 1, 42
};

seq_t a(idx_t n)
{
    /* Lazy initialization of the cache. */
    if (cache == NULL) {
        cache_cap   = 2;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory: return 0 */
        cache[0] = 1;
        cache[1] = 1;
        cache_size = 2;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_size up to n inclusive. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* coefficient for q[i] is cf_coeff[(i-1) % 14] because
           cf_coeff[0] corresponds to a[1] in the continued fraction. */
        int  c   = cf_coeff[(i - 1) % 14];
        cache[i] = ((seq_t)c) * cache[i - 1] + cache[i - 2];
    }
    cache_size = n + 1;
    return cache[n];
}
