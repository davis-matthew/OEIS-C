/*
 * A166289  Number of Dyck paths with no UUU's and no DDD's, of semilength n
 *          and having no UDUD's (U=(1,1), D=(1,-1)).
 * https://oeis.org/A166289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A166289:
 * (n+3)*a(n) + (n+1)*a(n-1) - 2*n*a(n-2) + 2*(-3*n+5)*a(n-3)
 * + (-3*n+11)*a(n-4) + (n-5)*a(n-5) = 0 ;
 * Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2.
 */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_next = 0;   /* number of already computed elements */

/* Ensure the cache has at least `needed_cap` slots and initialize
 * the base values if this is the first call. */
static void ensure_cache(size_t needed_cap)
{
    size_t required = needed_cap;
    if (required < 5) required = 5;            /* we need space for the 5 base terms */

    if (cache_cap < required) {
        seq_t *new_cache = (seq_t*)malloc(required * sizeof(seq_t));
        if (new_cache == NULL) return;         /* allocation failure → return */

        if (cache != NULL) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = required;
    }

    if (cache_next == 0) {
        /* initialise the first five terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)2;
        cache_next = 5;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx + 1);

    if (idx < cache_next)  /* already computed */
        return cache[idx];

    for (size_t k = cache_next; k <= idx; ++k) {
        idx_t m = (idx_t)k;

        /* compute the numerator of the recurrence */
        seq_t term2 = (seq_t)(2 * m) * cache[m - 2];
        seq_t term3 = (seq_t)(6 * m - 10) * cache[m - 3];
        seq_t term4 = (seq_t)(3 * m - 11) * cache[m - 4];
        seq_t pos_total = term2 + term3 + term4;

        seq_t term1 = (seq_t)(m + 1) * cache[m - 1];
        seq_t term5 = (seq_t)(m - 5) * cache[m - 5];   /* zero when m==5 */
        seq_t neg_total = term1 + term5;

        seq_t numerator = pos_total - neg_total;
        seq_t value = numerator / (seq_t)(m + 3);
        cache[k] = value;
    }

    cache_next = idx + 1;
    return cache[idx];
}
