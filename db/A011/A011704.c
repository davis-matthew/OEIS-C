/*
 * A011704  A binary m-sequence: expansion of reciprocal of x^8+x^6+x^3+x^2+1.
 * https://oeis.org/A011704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the binary m‑sequence (reciprocal of
   x^8 + x^6 + x^3 + x^2 + 1 over GF(2):
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=1
   a(n)=a(n-2) XOR a(n-3) XOR a(n-6) XOR a(n-8)   for n≥8 */
#include <stdlib.h>

static seq_t *cache      = NULL;   /* stores computed values   */
static size_t cache_cap  = 0;      /* current cache capacity   */

/* Ensure cache is large enough to contain a(n) for all n ≤ target. */
static void ensure_cache(idx_t target_idx)
{
    /* Determine required capacity, at least as large as the first 8
       initial conditions.                                     */
    size_t target_cap = (size_t)(target_idx + 1);
    if (target_cap < 8)
        target_cap = 8;

    if (cache_cap >= target_cap)
        return;                           /* nothing to do            */

    /* Grow the cache.  Use realloc so the existing data is preserved. */
    size_t old_cap   = cache_cap;
    cache          = (seq_t *)realloc(cache, target_cap * sizeof(seq_t));
    if (cache == NULL)                     /* allocation failed        */
        return;                           /* caller will fail slow   */
    cache_cap = target_cap;

    if (old_cap == 0) {                    /* first allocation        */
        /* Initialise the first eight terms.                   */
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0;
        cache[4] = 0; cache[5] = 0; cache[6] = 0; cache[7] = 1;

        /* Generate the remaining terms.                        */
        for (size_t i = 8; i < cache_cap; ++i)
            cache[i] =
                cache[i-2] ^ cache[i-3] ^ cache[i-6] ^ cache[i-8];
    } else {                               /* grow existing cache    */
        /* The first eight entries are already correct.  Generate
           new entries from old_cap up to cache_cap-1.         */
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] =
                cache[i-2] ^ cache[i-3] ^ cache[i-6] ^ cache[i-8];
    }
}

/* --------------------------------------------------------------- */
/*  Public function requested by the contest.                       */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
