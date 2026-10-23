/*
 * A183597  1/6 the number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          containing all three values.
 * https://oeis.org/A183597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A183597:  a(n) = 12*a(n-1)+20*a(n-2)-238*a(n-3)-420*a(n-4)+324*a(n-5)+400*a(n-6)-224*a(n-7);
   Initial values: a(1)=120, a(2)=1479, a(3)=17174, a(4)=201770, a(5)=2366412,
   a(6)=27768032, a(7)=325834456. */

#include <stdlib.h>

static seq_t *cache     = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_last = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; behaviour for n<1 is undefined */

    /* Initialise cache with known terms if first call */
    if (cache == NULL) {
        /* Ensure space for at least the first 7 terms */
        size_t init_cap = (n + 1 > 8 ? n + 1 : 8);
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure */
        cache_cap  = init_cap;
        /* Set the first seven terms */
        cache[1] = 120;
        cache[2] = 1479;
        cache[3] = 17174;
        cache[4] = 201770;
        cache[5] = 2366412;
        cache[6] = 27768032;
        cache[7] = 325834456;
        /* Largest computed index so far */
        cache_last = 7;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap  = cache_cap;
        size_t new_cap  = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure */
        for (size_t i = 0; i < old_cap; ++i) new_cache[i] = cache[i];
        free(cache);
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            seq_t pos = a1 * 12 + a2 * 20 + a5 * 324 + a6 * 400;
            seq_t neg = a3 * 238 + a4 * 420 + a7 * 224;
            cache[i] = pos - neg;
        }
        cache_last = n;
    }

    return cache[n];
}
