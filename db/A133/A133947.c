/*
 * A133947  a(n) = the number of "non-isolated divisors" of n(n+1). A positive
 *          divisor, k, of n is non-isolated if (k-1) or (k+1) also divides n.
 * https://oeis.org/A133947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of non-isolated divisors of n(n+1)
 * Range:     u8 n <= 15, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_non_isolated, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/count_non_isolated.h"

static seq_t *cache = NULL;   /* memoisation table */
static size_t cache_cap = 0;  /* current capacity of cache */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe */

    /* lazy cache expansion */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 2 : cache_cap;
        while (new_cap < need)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }
    if (cache[n] != 0)
        return cache[n];

    /* compute m = n*(n+1) as seq_t to use % operator later */
    seq_t m = (seq_t)n * ((seq_t)n + 1);

    /* factor n and n+1 (they are coprime) */
    uint64_t primes_n[20], exps_n[20];
    uint64_t primes_np1[20], exps_np1[20];
    size_t cnt_n = 0, cnt_np1 = 0;

    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);
    factor_u64((uint64_t)n + 1, primes_np1, exps_np1, &cnt_np1);

    /* merge the two factorizations into a single list */
    uint64_t primes[40], exps[40];
    size_t pf_cnt = 0;
    for (size_t i = 0; i < cnt_n; ++i) {
        primes[pf_cnt] = primes_n[i];
        exps[pf_cnt]   = exps_n[i];
        ++pf_cnt;
    }
    for (size_t i = 0; i < cnt_np1; ++i) {
        primes[pf_cnt] = primes_np1[i];
        exps[pf_cnt]   = exps_np1[i];
        ++pf_cnt;
    }

    /* enumerate all divisors and count the non-isolated ones */
    uint32_t cnt = 0;
    count_non_isolated(0, 1, primes, exps, pf_cnt, m, &cnt);

    cache[n] = (seq_t)cnt;
    return cache[n];
}
