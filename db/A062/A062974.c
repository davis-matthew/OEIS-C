/*
 * A062974  Numbers k such that omega(k+1) < 2*omega(k), where omega(k) is the
 *          number of distinct prime divisors of k.
 * https://oeis.org/A062974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A062974 – Numbers k such that ω(k+1) < 2·ω(k)
 * The function searches forward until the n‑th qualifying k is found, 
 * caching earlier results for fast random access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

static seq_t *cache = NULL;   /* cached qualifying k values */
static size_t cache_len = 0;  /* number of entries currently cached   */
static size_t cache_cap = 0;  /* capacity of the cache array          */
static uint64_t last_k = 1;   /* last candidate tested (start before 2) */

/* ensure cache can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    while (cache_len < n)
    {
        /* probe next integer k */
        last_k++;                     /* next candidate */
        uint64_t k = last_k;

        /* compute ω(k) and ω(k+1) */
        int omega_k   = count_distinct_primes(k);
        int omega_next = count_distinct_primes(k + 1);

        /* check the defining inequality */
        if (omega_next < 2 * omega_k)
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)k;   /* store the qualifying k */
        }
    }
    return cache[n - 1];
}
