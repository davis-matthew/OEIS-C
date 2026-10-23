/*
 * A123592  Primes of the form p^2 + q^2 + r^2, where p,q,r are primes.
 * https://oeis.org/A123592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A123592: primes that can be written as p^2 + q^2 + r^2 with
    p, q, r being primes.  The function computes the n‑th term
    (1‑indexed) by generating all sums of the form 9 + a^2 + b^2,
    where a and b are primes (at least one of the three primes in
    the representation is 3).  The sums that are prime are collected,
    sorted, deduplicated and cached.  This guarantee correct
    behaviour for any n, even if calls are not monotonic. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* local compare routine for qsort on seq_t values */
static int seq_compare(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

/* cache of sequence values: 1‑indexed, cache[0] is a(1) */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static idx_t  cache_limit = 0;      /* prime limit used to build cache */

/* rebuild the cache using all primes ≤ new_limit */
static void rebuild_cache(idx_t new_limit)
{
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;

    /* find all primes up to new_limit */
    build_primes_upto(new_limit, &primes, &primes_cnt);

    /* collect prime sums 9 + a^2 + b^2 */
    seq_t *tmp = NULL;
    size_t tmp_cap = 0;
    size_t tmp_len = 0;

    for (size_t i = 0; i < primes_cnt; i++) {
        seq_t ai_sq = (seq_t)primes[i] * (seq_t)primes[i];
        for (size_t j = i; j < primes_cnt; j++) {
            seq_t bj_sq = (seq_t)primes[j] * (seq_t)primes[j];
            seq_t s = (seq_t)9 + ai_sq + bj_sq;
            if (is_prime_big(s)) {
                if (tmp_len >= tmp_cap) {
                    size_t new_cap = (tmp_cap == 0) ? 16 : tmp_cap * 2;
                    tmp = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
                    tmp_cap = new_cap;
                }
                tmp[tmp_len++] = s;
            }
        }
    }
    free(primes);

    /* sort and deduplicate */
    qsort(tmp, tmp_len, sizeof(seq_t), seq_compare);

    size_t uniq_len = 0;
    if (tmp_len > 0)
        uniq_len = 1;   /* at least one element */
    for (size_t i = 1; i < tmp_len; i++)
        if (tmp[i] != tmp[i - 1])
            uniq_len++;

    seq_t *new_cache = (seq_t *)malloc(uniq_len * sizeof(seq_t));
    if (!new_cache) {
        free(tmp);
        return;
    }

    size_t pos = 0;
    if (tmp_len > 0)
        new_cache[pos++] = tmp[0];
    for (size_t i = 1; i < tmp_len; i++)
        if (tmp[i] != tmp[i - 1])
            new_cache[pos++] = tmp[i];

    free(tmp);

    /* replace old cache */
    free(cache);
    cache = new_cache;
    cache_size = uniq_len;
    cache_limit = new_limit;
}

/* ensure that the cache holds at least n terms */
static void ensure_terms(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;                /* already have enough */

    idx_t limit = 1000;
    if (cache_limit > 0)
        limit = cache_limit;   /* start from previously used limit */

    while (cache_size < (size_t)n) {
        rebuild_cache(limit);
        if (cache_size >= (size_t)n)
            break;
        if (limit > UINT64_MAX / 2)
            break;          /* cannot grow further */
        limit *= 2;
    }
}

/* returns the n‑th term of A123592 (1‑indexed) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, but return 0 */

    ensure_terms(n);
    return cache[n - 1];
}
