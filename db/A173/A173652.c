/*
 * A173652  Expansion of g.f.: x^3*(1 + 4*x - x^2 - 6*x^3 + x^4)/(1 - 9*x^2 -
 *          3*x^3 + 17*x^4 + 8*x^5 - 6*x^6 - 7*x^7 + x^8 - x^9).
 * https://oeis.org/A173652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:     a(0)=0, a(1)=0, a(2)=0, a(3)=1,
                    a(4)=4, a(5)=8, a(6)=33, a(7)=68, a(8)=245;
                    For n≥9:
                      a(n)=9*a(n-2)+3*a(n-3)-17*a(n-4)-8*a(n-5)
                           +6*a(n-6)+7*a(n-7)-a(n-8)+a(n-9)
   This follows from the generating function:
   x^3*(1+4x-x^2-6x^3+x^4)/(1-9x^2-3x^3+17x^4+8x^5-6x^6-7x^7+x^8-x^9) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  computed   = 0; /* number of filled entries */

static void
grow_cache(idx_t n)
{
    /* Ensure cache can hold indices 0..n inclusive */
    if ((idx_t)(n + 1) <= cache_len)
        return;
    idx_t new_len = (cache_len > 0) ? cache_len * 2 : 16;
    if (new_len < n + 1)
        new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure – unlikely in contest setting */
    cache = tmp;
    cache_len = new_len;
}

static const seq_t init_vals[9] = {
    0, 0, 0, 1, 4, 8, 33, 68, 245
};

seq_t
a(idx_t n)
{
    if (n < 9)
        return init_vals[n];

    /* One‑time initialization of the cache with the seed values */
    if (computed == 0) {
        grow_cache(8);
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        computed = 9;
    }

    /* Grow cache if required */
    grow_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = (seq_t)9  * cache[i - 2]
                 + (seq_t)3  * cache[i - 3]
                 - (seq_t)17 * cache[i - 4]
                 - (seq_t)8  * cache[i - 5]
                 + (seq_t)6  * cache[i - 6]
                 + (seq_t)7  * cache[i - 7]
                 -          cache[i - 8]
                 +          cache[i - 9];
    }
    computed = n + 1;  /* all indices up to n are now stored */

    return cache[n];
}
