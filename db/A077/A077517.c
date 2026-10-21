/*
 * A077517  a(1) = 1 and a(n+1) = smallest multiple of a(n) beginning in
 *          (n+1).
 * https://oeis.org/A077517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = minimal integer m such that m is a multiple of a(n) and
   the decimal representation of m starts with (n+1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based array, cache[1] = a(1) */
    static idx_t cache_len = 1;   /* number of terms currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity (>= cache_len+1) */

    /* Initialise cache when first called */
    if (cache == NULL) {
        cache_cap = 2;                          /* capacity for indices 0..1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;                    /* a(1) = 1 */
    }

    /* Grow the cache if we need to compute more terms */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        /* copy existing data */
        for (idx_t i = 1; i <= cache_len; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];           /* a(i-1) */
        seq_t target = (seq_t)i;               /* the prefix (i) */
        seq_t pow10  = (seq_t)1;               /* 10^t */

        for (;;) {
            seq_t lower = target * pow10;                   /* (n+1)*10^t */
            seq_t upper = ((seq_t)(i + 1) * pow10) - (seq_t)1; /* ((n+2)*10^t)-1 */
            /* ceil division: (lower + prev - 1) / prev */
            seq_t k   = (lower + prev - (seq_t)1) / prev;
            seq_t cand = k * prev;                          /* candidate multiple */
            if (cand <= upper) {
                cache[i] = cand;
                break;
            }
            /* increase t: multiply pow10 by 10 */
            pow10 *= (seq_t)10;
        }
    }
    cache_len = n;
    return cache[n];
}
