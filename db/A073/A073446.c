/*
 * A073446  Product L(n)*S(n), where L(n) are Lucas numbers and S(n) are Lucas
 *          3-step numbers = A000032(n) * A001644(n).
 * https://oeis.org/A073446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+4*a(n-2)+5*a(n-3)+2*a(n-4)-a(n-5)+a(n-6) a(0)=6,
 *            a(1)=1, a(2)=9, a(3)=28, a(4)=77, a(5)=231 This code memoises
 *            computed terms to answer calls in any order
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* memoisation table */
static idx_t cache_size = 0;   /* number of elements stored (max index + 1) */
static idx_t cache_cap  = 0;   /* allocated size */

/* grow the memoisation table until it can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise with first six terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)28;
        cache[4] = (seq_t)77;
        cache[5] = (seq_t)231;
        cache_size = 6;
    }
    /* expand if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1) new_cap <<= 1;   /* power‑of‑two growth */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return;                   /* allocation failed – leave cache as is */
        cache = new_cache;
        cache_cap = new_cap;
    }
    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)+4*a(i-2)+5*a(i-3)+2*a(i-4)-a(i-5)+a(i-6) */
        seq_t val = cache[i-1];
        val += (cache[i-2] * ((seq_t)4));
        val += (cache[i-3] * ((seq_t)5));
        val += (cache[i-4] * ((seq_t)2));
        val += cache[i-6];
        val -= cache[i-5];
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 6) {
        static const seq_t init[6] = {
            (seq_t)6,
            (seq_t)1,
            (seq_t)9,
            (seq_t)28,
            (seq_t)77,
            (seq_t)231
        };
        return init[n];
    }
    ensure_cache(n);
    return cache[n];
}
