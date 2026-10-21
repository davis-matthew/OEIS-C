/*
 * A053318  a(n) contains n digits (either '2' or '7') and is divisible by
 *          2^n.
 * https://oeis.org/A053318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(2 + 5*((a(n-1)/2^(n-1)) mod 2)), with
 *            a(1) = 2 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   grow_seq_array, pow10, pow2, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/pow2.h"
#include "../utils/grow_seq_array.h"
#include "../utils/seq_alloc_new_array.h"

/* static cache for memoisation */
static seq_t *cache          = NULL;   /* indices 0 .. cache_cap-1 */
static size_t cache_cap      = 0;      /* allocated capacity   */
static size_t cache_max      = 0;      /* highest index computed */

/* a(n) --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* below offset 1: behaviour not required, return 0 */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                     /* indices 0 and 1 used */
        cache = seq_alloc_new_array(cache_cap);   /* allocate cache */
        cache[0] = (seq_t)0;               /* dummy entry for 0 */
        cache[1] = (seq_t)2;               /* a(1) = 2       */
        cache_max = 1;
    }

    /* grow cache if next index exceeds current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* compute missing terms up to requested n */
    for (idx_t i = (idx_t)cache_max + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];                 /* a(i-1)          */
        seq_t pow2v  = pow2((idx_t)(i - 1));          /* 2^(i-1)         */
        seq_t q      = prev / pow2v;                  /* floor division  */
        seq_t odd    = q & (seq_t)1;                  /* q mod 2         */
        seq_t coeff  = (seq_t)2 + (seq_t)5 * odd;     /* 2 + 5*odd       */
        seq_t pow10v = pow10((idx_t)(i - 1));         /* 10^(i-1)        */
        cache[i] = prev + pow10v * coeff;             /* recurrence      */
    }

    cache_max = (size_t)n;
    return cache[n];
}
