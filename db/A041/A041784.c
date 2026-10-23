/*
 * A041784  Numerators of continued fraction convergents to sqrt(413).
 * https://oeis.org/A041784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041784: numerators of continued‑fraction convergents to sqrt(413).  
   Recurrence: a(0)=20, a(1)=61; for n≥2,
   a(n)=period[(n-1)%8]*a(n-1)+a(n-2), where period = {3,9,1,4,1,9,3,40}. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* partial quotients of the continued fraction, period 8 */
static const seq_t period[8] = {
    (seq_t)3,(seq_t)9,(seq_t)1,(seq_t)4,
    (seq_t)1,(seq_t)9,(seq_t)3,(seq_t)40
};

static void ensure_cache(idx_t up_to)
{
    /* already cached enough */
    if (cache_len > 0 && up_to < cache_len)
        return;

    /* initialise cache when first needed */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)20;
        cache[1] = (seq_t)61;
        cache_len = 2;
    }

    /* grow cache while computing new terms */
    for (idx_t k = cache_len; k <= up_to; ++k) {
        seq_t term = period[(k-1)%8] * cache[k-1] + cache[k-2];

        if (cache_len + 1 > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
            while (new_cap <= cache_len + 1)
                new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = term;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
