/*
 * A146566  Numbers k such that k*sigma_0(k) is divisible by (k - sigma_0(k)).
 * https://oeis.org/A146566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A146566:
 * Numbers k such that k * σ₀(k) is divisible by (k - σ₀(k)).
 *
 * The function generates terms lazily and caches them, so calls with
 * arbitrary order of the index n are supported.  The cache stores the
 * index values (k) in a standard C array; the return value is
 * cast to the seq_t type supplied by the build.  Only integer
 * arithmetic, including use of unsigned __int128 for the interim
 * product, is required.  No floating‑point or external libraries
 * beyond the optional factorisation helper are used.  The code
 * compiles under both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used in OEIS */

    /* lazily‑filled cache of k‑values */
    static idx_t *cache  = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 1;              /* start before first candidate */

    /* return cached value if already computed */
    if (n <= cache_len)
        return (seq_t)cache[n - 1];

    /* initial allocation of the cache */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
    }

    /* generate further terms until we have at least n of them */
    while (cache_len < n) {
        ++last_k;                         /* test next integer */

        /* divisor count σ₀(last_k) via prime factorisation */
        uint64_t primes[64], exps[64];
        size_t pf_cnt;
        factor_u64((uint64_t)last_k, primes, exps, &pf_cnt);

        idx_t d = 1;
        for (size_t i = 0; i < pf_cnt; ++i)
            d *= (idx_t)(exps[i] + 1);

        idx_t diff = last_k - d;
        if (diff > 0) {
            /* check (last_k * d) % diff == 0 */
            unsigned __int128 prod = (unsigned __int128)last_k * d;
            if ((prod % diff) == 0) {
                /* store the term, growing the cache if needed */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
                }
                cache[cache_len++] = last_k;
            }
        }
    }

    return (seq_t)cache[n - 1];
}
