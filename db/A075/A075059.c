/*
 * A075059  a(n) = 1 + lcm(1, 2, ..., n) = 1 + A003418(n).
 * https://oeis.org/A075059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + LCM(1,2,…,n) (OEIS A075059)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores previously computed terms in a static array
   and extends it on demand.  Each new term is obtained from the
   previous lcm value by LCM(prev_lcm,n) = prev_lcm / gcd(prev_lcm,n) * n.
   The helper function gcd() is taken from the existing utilities. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;     /* array holding a(0), a(1), … */
static idx_t cache_size = 0;    /* number of computed entries (may be 0) */

/* Extend the cache so that it contains entries up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                 /* already available */

    idx_t new_size = n + 1;                     /* need entries 0…n */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);         /* allocation failure */

    /* Initialize first term if cache was empty */
    if (cache_size == 0) {
        new_cache[0] = (seq_t)2;                /* a(0) = 2 */
        cache_size = 1;
    }

    /* Compute missing entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev_lcm = new_cache[i-1] - (seq_t)1;            /* a(i-1)-1 */
        seq_t g = gcd(prev_lcm, (seq_t)i);                    /* gcd(prev_lcm,i) */
        seq_t div = prev_lcm / g;                             /* previous lcm / gcd */
        seq_t new_lcm = div * (seq_t)i;                       /* lcm(prev_lcm,i) */
        new_cache[i] = new_lcm + (seq_t)1;                    /* a(i) */
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
