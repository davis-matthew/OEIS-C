/*
 * A203882  Number of (n+1) X 4 0..6 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 79*a(n-1) - 450*a(n-2) - 144*a(n-3) (n ≥ 4) Initial
 *            values: a(1)=107653, a(2)=7501753, a(3)=543848893
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   mul144, mul450, mul79
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul144.h"
#include "../utils/mul450.h"
#include "../utils/mul79.h"

/* ----- cache handling ----- */
static seq_t *cache        = NULL;   /* cache[i-1] = a(i)  (1‑based indices) */
static idx_t   cache_cap   = 0;     /* allocated capacity (number of elements) */
static idx_t   cache_size  = 0;     /* highest computed index whose value is stored */

static void enlarge_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = needed;
    /* realloc copies the existing elements automatically */
    cache = (seq_t *)realloc(cache,
                              sizeof(seq_t) * (size_t)new_cap);
    cache_cap = new_cap;
}

/* ----- sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1)        /* undefined for n < 1 */
        return (seq_t)0;

    /* initialise cache with the first three terms if necessary */
    if (cache_size == 0) {
        enlarge_cache(3);
        cache[0] = (seq_t)107653;      /* a(1) */
        cache[1] = (seq_t)7501753;     /* a(2) */
        cache[2] = (seq_t)543848893;   /* a(3) */
        cache_size = 3;
    }

    /* already computed? */
    if (n <= cache_size)
        return cache[n - 1];

    /* ensure we have enough room for all terms up to n */
    enlarge_cache(n);

    /* compute missing terms sequentially */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* indices 1‑based -> cache array 0‑based */
        seq_t t1 = cache[i - 2];   /* a(i-1) */
        seq_t t2 = cache[i - 3];   /* a(i-2) */
        seq_t t3 = cache[i - 4];   /* a(i-3) */
        cache[i - 1] = mul79(t1) - mul450(t2) - mul144(t3);
    }
    cache_size = n;
    return cache[n - 1];
}
