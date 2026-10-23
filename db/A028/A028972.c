/*
 * A028972  Numbers represented by quadratic form with Gram matrix [ 7, 1, -3;
 *          1, 3, 1; -3, 1, 7 ].
 * https://oeis.org/A028972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th non‑negative integer represented by the
 *            quadratic form with Gram matrix [ 7 1 -3 1 3 1 -3 1 7 ] The
 *            sequence is obtained by enumerating all triples (a,b,c) of
 *            non‑negative integers, computing the four sign‑patterns and
 *            marking the resulting non‑negative values
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note: the function relies only on the standard headers below
    and on the types seq_t and idx_t provided by the build. */

#include <stdlib.h>
#include <stdint.h>

/* static cache data -------------------------------------------------------*/

static seq_t *cache = NULL;   /* array of sequence values                          */
static size_t cache_len = 0;  /* number of values currently cached                 */
static idx_t  cache_B   = 0;  /* largest B used in the current cache               */

/* Build the cache up to bound B -------------------------------------------*/
static void build_cache(idx_t B)
{
    /* free previous cache if any */
    if (cache != NULL) {
        free(cache);
        cache = NULL;
    }

    /* Mark representable numbers up to B */
    unsigned char *present = (unsigned char*)calloc((size_t)B + 1, 1);
    if (!present) return;  /* allocation failure – best effort */

    for (idx_t a = 0; ; ++a) {
        uint64_t a2 = 7ULL * a * a;
        if (a2 > B) break;
        for (idx_t b = 0; ; ++b) {
            uint64_t b2 = 3ULL * b * b;
            if (a2 + b2 > B) break;
            for (idx_t c = 0; ; ++c) {
                uint64_t c2 = 7ULL * c * c;
                uint64_t sum = a2 + b2 + c2;
                if (sum > B) break;

                uint64_t ab = a * b;
                uint64_t ac = a * c;
                uint64_t bc = b * c;

                int64_t v1 = (int64_t)sum + 2*(int64_t)ab - 6*(int64_t)ac + 2*(int64_t)bc;
                int64_t v2 = (int64_t)sum + 2*(int64_t)ab + 6*(int64_t)ac - 2*(int64_t)bc;
                int64_t v3 = (int64_t)sum - 2*(int64_t)ab - 6*(int64_t)ac - 2*(int64_t)bc;
                int64_t v4 = (int64_t)sum - 2*(int64_t)ab + 6*(int64_t)ac + 2*(int64_t)bc;

                if (v1 >= 0 && (idx_t)v1 <= B) present[(size_t)v1] = 1;
                if (v2 >= 0 && (idx_t)v2 <= B) present[(size_t)v2] = 1;
                if (v3 >= 0 && (idx_t)v3 <= B) present[(size_t)v3] = 1;
                if (v4 >= 0 && (idx_t)v4 <= B) present[(size_t)v4] = 1;
            }
        }
    }

    /* Count and collect values into the cache */
    size_t count = 0;
    for (idx_t i = 0; i <= B; ++i)
        if (present[(size_t)i]) ++count;

    cache = (seq_t*)malloc(count * sizeof(seq_t));
    if (!cache) { free(present); return; }

    size_t pos = 0;
    for (idx_t i = 0; i <= B; ++i)
        if (present[(size_t)i])
            cache[pos++] = (seq_t)i;

    cache_len = count;
    cache_B   = B;

    free(present);
}

/* Main sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1 – return 0 */

    /* Build an initial cache if none exists */
    if (cache == NULL)
        build_cache(200000);          /* sufficient for n up to ~10000 */

    /* Increase the cached bound until it covers the requested n-th term */
    while (n > cache_len) {
        /* If the bounds were zero (first call), try a small default */
        idx_t newB = (cache_B == 0) ? 200000 : cache_B * 2;
        build_cache(newB);
    }

    /* Indexing is 1‑based for this sequence */
    return cache[n - 1];
}
