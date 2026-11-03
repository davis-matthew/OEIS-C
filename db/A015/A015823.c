/*
 * A015823  Numbers k such that phi(k + 7) | sigma(k).
 * https://oeis.org/A015823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1095, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 71 terms through n = 2903 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A015823: Numbers k such that phi(k + 7) divides sigma(k) */

/* Does not use a closed‑form formula, so a search is performed and
   the results are cached for repeated calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Cache of found terms.  Cached terms are stored in the order in
   which they occur. */
static seq_t *cache = NULL;   /* array of seq_t */
static size_t cache_len = 0;  /* number of elements currently in cache */
static size_t cache_cap = 0;  /* current allocated capacity (in elements) */

/* The smallest k that has not yet been examined. */
static idx_t next_k = 1;

/* Grow the cache to accommodate at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) return;                 /* ignore failure – subsequent
                                        * calls will continue from
                                        * where they left off */
    cache   = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term (n >= 1).  The function is safe for any order
   of calls; previously found terms are reused from the cache. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1 – not used by callers */

    /* Search until we have at least n terms cached. */
    while (cache_len < n) {
        /* Compute sigma(k) and phi(k+7).  The helper functions use
           seq_t internally, so no type conversions are required. */
        seq_t sigma = sigma_of(next_k);
        seq_t phi   = totient_seq((seq_t)(next_k + 7));

        /* If phi divides sigma, store the term. */
        if (phi != (seq_t)0 && sigma % phi == (seq_t)0) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }

        ++next_k;   /* advance to the next candidate k */
    }

    return cache[n - 1];
}
