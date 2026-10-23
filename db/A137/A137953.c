/*
 * A137953  G.f. satisfies A(x) = 1 + x*(1 + x*A(x)^2)^3.
 * https://oeis.org/A137953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   seq_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A137953
 * a(0) = 1
 * a(n) = Sum_{k=0..n-1} C(3*(n-k),k)/(n-k) * C(2*k,n-k-1)  for n>0
 * The function memoises values in a static cache so calls may be in any order. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_binom.h"

/* Static cache to store computed terms. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t up_to)
{
    if (cache_len > up_to) return;

    idx_t new_len = up_to + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failure */
    cache = new_cache;

    /* Compute new terms. */
    for (idx_t m = cache_len; m <= up_to; ++m) {
        if (m == 0) { cache[0] = 1; continue; }

        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t nk = m - k;                      /* n-k   */
            seq_t t1 = seq_binom(3 * nk, k);        /* binom(3*(n-k),k) */
            seq_t t2 = seq_binom(2 * k, nk - 1);    /* binom(2*k,n-k-1) */
#if defined(__SIZEOF_INT128__)
            if (sizeof(seq_t) <= 8) {
                sum += ( (unsigned __int128)t1 * t2 ) / nk;
            } else
#endif
            {
                sum += (t1 * t2) / (seq_t)nk;
            }
        }
        cache[m] = sum;
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
