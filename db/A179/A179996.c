/*
 * A179996  Composite numbers n such that Sum_{k=0..q} d(k)*10^k is prime
 *          where d(k) are the q+1 distinct prime factors of n, d(0)< d(1) <
 *          ... < d(q).
 * https://oeis.org/A179996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   factor_u64, is_prime, is_prime_big, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  Search composite numbers with at least two distinct prime factors.
   For each such number n, compute v = Σ d(k) · 10^k where d(k) are the distinct
   prime factors of n in increasing order.  If v is prime, n is part of the
   sequence.  The function returns the n‑th such composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 4;

/* Ensure that the cache contains at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_len)
        return;

    /* Allocate or grow the cache if necessary */
    if (!cache) {
        cache_cap = (need < 16) ? 16 : need;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    } else if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Factorization buffers */
    const size_t MAX_FACTS = 64;
    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt;

    /* Generate terms until we have enough */
    while (cache_len < need) {
        uint64_t n = next_candidate++;
        /* Factor n */
        factor_u64(n, primes, exps, &cnt);
        /* Need at least two distinct prime factors */
        if (cnt < 2)
            continue;

        /* Build the concatenated number v = Σ primes[i] * 10^i */
        seq_t v = 0;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t term = seq_mul((seq_t)primes[i], pow10((idx_t)i));
            v += term;
        }

        /* Test primality of v */
        if (is_prime_big(v))
            cache[cache_len++] = (seq_t)n;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
