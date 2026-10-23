/*
 * A041640  Numerators of continued fraction convergents to sqrt(339).
 * https://oeis.org/A041640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A041640: numerators of convergents to sqrt(339)
   Recurrence:  p(-1) = 1,
                p(0) = 18,
                a_1 … a_10 = {2,2,2,1,17,1,2,2,2,36}
                p(n) = a_n * p(n-1) + p(n-2)   for n ≥ 1
   a(n) = p(n)  (a(0)=18, a(1)=37, …)
   The coefficients a_n are periodic with period 10. */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for already computed numerators */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* initialise first term if this is the first call */
    if (cache_len == 0) {
        cache[0] = 18;            /* a(0) */
        cache_len = 1;
    }

    /* start recurrence from the last cached values */
    seq_t p_nm2, p_nm1;
    if (cache_len == 1) {
        p_nm2 = 1;                /* p(-1) */
        p_nm1 = cache[0];         /* p(0)  */
    } else {
        p_nm2 = cache[cache_len - 2];
        p_nm1 = cache[cache_len - 1];
    }

    /* period of the continued‑fraction coefficients of sqrt(339) */
    static const unsigned period[10] = {
        2, 2, 2, 1, 17, 1, 2, 2, 2, 36
    };

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        unsigned coef;
        if (k == 0) {
            coef = 18; /* corresponds to a(0) */
        } else {
            coef = period[(k - 1) % 10];
        }
        seq_t pk = p_nm1 * coef + p_nm2;
        cache[k] = pk;
        p_nm2 = p_nm1;
        p_nm1 = pk;
    }

    cache_len = n + 1;
    return cache[n];
}
