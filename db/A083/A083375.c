/*
 * A083375  n appears prime(n) times.
 * https://oeis.org/A083375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A083375: n appears prime(n) times.  Compute a(n). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes (prime(i) = primes[i]) and cumulative sums */
    static uint64_t *primes = NULL;
    static size_t   primes_cnt = 0;
    static seq_t   *cum  = NULL;
    static idx_t    limit = 200000;   /* numeric upper bound for primes */
    static int      init   = 0;

    /* one‑time initialization (or extension if needed) */
    if (!init) {
        uint64_t *tmp_p = NULL;
        size_t tmp_c = 0;
        build_primes_upto(limit, &tmp_p, &tmp_c);
        primes = tmp_p;
        primes_cnt = tmp_c;

        cum = (seq_t *)malloc(primes_cnt * sizeof(seq_t));
        if (!cum) return (seq_t)0; /* OOM guard */
        for (size_t i = 0; i < primes_cnt; ++i) {
            cum[i] = (i == 0) ? (seq_t)primes[i] :
                     cum[i-1] + (seq_t)primes[i];
        }
        init = 1;
    }

    /* if n exceeds current cumulative limit, extend cache */
    while (n > cum[primes_cnt - 1]) {
        limit *= 2;
        uint64_t *tmp_p = NULL;
        size_t tmp_c = 0;
        build_primes_upto(limit, &tmp_p, &tmp_c);
        free(primes);
        free(cum);
        primes = tmp_p;
        primes_cnt = tmp_c;
        cum = (seq_t *)malloc(primes_cnt * sizeof(seq_t));
        if (!cum) return (seq_t)0; /* OOM guard */
        for (size_t i = 0; i < primes_cnt; ++i) {
            cum[i] = (i == 0) ? (seq_t)primes[i] :
                     cum[i-1] + (seq_t)primes[i];
        }
    }

    /* binary search over cumulative sums to locate the block containing n */
    idx_t lo = 0, hi = (idx_t)primes_cnt - 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }

    /* lo is the index (0‑based) of the number that occupies position n */
    return (seq_t)(lo + 1);
}
