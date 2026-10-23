/*
 * A048435  Take the first n numbers written in base 3, concatenate them, then
 *          convert from base 3 to base 10.
 * https://oeis.org/A048435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   digits_in_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A048435
    a(1) = 1
    a(n) = a(n-1) * 3^{digits₃(n)} + n   (n ≥ 2)
    where digits₃(n) is the number of base‑3 digits of n.
    The function memoises results so that calls in random order are O(1)
    after the first computation of each required index. */

#include <stdlib.h>
#include <string.h>
#include "../utils/digits_in_base3.h"

static seq_t *cache = NULL;      /* memoised values  */
static idx_t  cache_cap = 0;     /* current allocated size   */
static idx_t  cache_size = 0;    /* highest n already computed */

/* Ensure that cache can hold indices up to 'need'.
   The array is indexed starting at 0; cache[0] is unused (set to 0). */
static void ensure_cache(idx_t need)
{
    if (need < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need + 1)
        new_cap = need + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)   /* allocation failed; abort with zero   */
        return;
    /* zero‑init the newly added slots */
    if (new_cap > cache_cap)
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute pow = 3^exp by repeated multiplication. */
static seq_t pow3(int exp)
{
    seq_t p = 1;
    for (int i = 0; i < exp; ++i)
        p *= 3;
    return p;
}

/* -------------------- Main function -------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* initialise cache if required */
    if (!cache) {
        cache_cap = 8;                       /* arbitrary start */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)                          /* allocation failure */
            return 0;
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 1;
    }

    /* extend cache to cover requested n */
    if (n >= cache_cap)
        ensure_cache(n);

    /* compute any missing values */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        int d = digits_in_base3(i);
        seq_t pow = pow3(d);
        cache[i] = cache[i - 1] * pow + (seq_t)i;
    }
    cache_size = n > cache_size ? n : cache_size;

    return cache[n];
}
