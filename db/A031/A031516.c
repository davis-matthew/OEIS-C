/*
 * A031516  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 18.
 * https://oeis.org/A031516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031516
   Numbers k such that the continued fraction for √k has
   an even period and, after dropping the last term of the period,
   the middle term equals 18.
   This implementation searches successive k and caches the found
   values so that calls to a(idx_t n) can be in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Use the types supplied by the harness. */
typedef idx_t   idx64;      /* idx_t is uint64_t in the harness */

/* ------------------------------------------------------------------ */
/* Integer square root for a 64‑bit integer, implemented without the
   <math.h> floating point functions.  The result is the floor of
   the square root. */
static idx64 isqrt_u64(idx64 n)
{
    idx64 lo = 0, hi = (idx64)1 << 32;   /* hi^2 ≥ 2^64 > n */
    while (lo < hi) {
        idx64 mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n) lo = mid;
        else                                   hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Cache for the sequence values.  We store the raw 64‑bit integer
   k values; the conversion to seq_t happens only when the function
   returns.  This strategy works for both builtin and arbitrary‑
   precision seq_t types. */
static uint64_t *cache_k   = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;

/* Append a new value to the cache, growing the buffer if necessary. */
static void cache_append(uint64_t k)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(cache_k,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        cache_k   = tmp;
        cache_cap = new_cap;
    }
    cache_k[cache_len++] = k;
}

/* ------------------------------------------------------------------ */
/* Main function required by the harness. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    static idx64 next_k = 1;           /* next candidate k to test */

    while (cache_len < n) {
        idx64 k = next_k++;
        if (k == 0) continue;                 /* protection against overflow */

        /* Skip perfect squares; their continued fraction is periodic with
           period 1 (odd). */
        idx64 sqrtk = isqrt_u64(k);
        if (sqrtk * sqrtk == k) continue;

        /* Compute the continued fraction period of √k.  The period ends
           when a₀ == 2*sqrtk.  We store the period terms in a small
           static array; the overall period length is < 4096 for the range
           of k that occurs in this sequence. */
        idx64 m = 0, d = 1, a0 = sqrtk;
        idx64 period[4096];
        size_t plen = 0;

        while (1) {
            m = d * a0 - m;
            /* Use unsigned __int128 to avoid overflow when computing m*m. */
            idx64 mm = (idx64)((unsigned __int128)m * m);
            d = (k - mm) / d;
            a0 = (sqrtk + m) / d;

            period[plen++] = a0;
            if (a0 == 2 * sqrtk) break;
            /* Safety guard: if the period somehow exceeds the buffer */
            if (plen >= sizeof(period)/sizeof(period[0])) break;
        }

        /* Check the required properties: even length and central term 18. */
        if ((plen % 2 == 0) && (period[plen / 2 - 1] == 18))
            cache_append(k);
    }

    /* Return the requested term, converting from uint64_t to seq_t. */
    return (seq_t)cache_k[n - 1];
}
