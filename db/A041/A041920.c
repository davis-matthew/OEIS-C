/*
 * A041920  Numerators of continued fraction convergents to sqrt(482).
 * https://oeis.org/A041920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041920:  Numerators of continued‑fraction convergents to sqrt(482)
   Sequence defined by:  a(n) = 966*a(n-4) – a(n-8)
   With initial values
       a(0)=21,  a(1)=22,  a(2)=461,  a(3)=483,
       a(4)=20747, a(5)=21230, a(6)=445347, a(7)=466577
   The function is memoised; it works for arbitrary order of calls and
   for any idx_t/seq_t combination, using only integer arithmetic.  */

#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache      = NULL;    /* array holding the terms */
static size_t cache_len  = 0;       /* number of terms currently stored   */
static size_t cache_cap  = 0;       /* current allocated capacity          */

/* Ensure the cache can hold at least n+1 terms */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache can store up to the requested index */
    ensure_capacity(n);

    /* Base values for 0 <= i < 8 */
    static const seq_t init[8] = {
        21, 22, 461, 483,
        20747, 21230, 445347, 466577
    };

    /* Compute any missing values */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            /* a(i) = 966*a(i-4) – a(i-8) */
            seq_t term1 = (seq_t)966 * cache[i - 4];
            seq_t term2 = cache[i - 8];
            cache[i] = term1 - term2;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
