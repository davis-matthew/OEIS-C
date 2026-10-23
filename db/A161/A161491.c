/*
 * A161491  a(n) = number of primes p less than A000040(n) (the n-th prime)
 *          such that (p + 1)*(A000040(n) - 1) is a square.
 * https://oeis.org/A161491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_primes_upto, floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of primes p < p_n such that (p + 1) * (p_n - 1) is
 * a perfect square
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Cached data: 1‑indexed arrays */
static uint32_t *prime_list = NULL;    /* primes, prime_list[1] = 2 */
static seq_t   *result_cache = NULL;   /* a(n), 1‑indexed */
static unsigned char *computed = NULL; /* 0/1 flags, 1‑indexed */
static idx_t   max_cached = 0;         /* largest n computed so far */

/* Ensure that prime_list, result_cache and computed contain entries
 * for all indices up to n.  If necessary we rebuild the prime list
 * up to the n‑th prime.
 */
static void
ensure_capacity(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Build prime list up to the n‑th prime */
    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    idx_t limit = (idx_t)get_prime(n);          /* n‑th prime */
    build_primes_upto(limit, &tmp, &tmp_cnt);   /* tmp_cnt should be n */

    /* Allocate / extend prime_list */
    uint32_t *new_primes = (uint32_t *)realloc(prime_list, (n + 1) * sizeof(uint32_t));
    if (!new_primes)
        abort();
    prime_list = new_primes;

    for (idx_t i = max_cached + 1; i <= n; ++i)
        prime_list[i] = (uint32_t)tmp[i - 1];   /* tmp[0] corresponds to prime 2 */

    free(tmp);

    /* Extend result_cache */
    seq_t *new_cache = (seq_t *)realloc(result_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();
    result_cache = new_cache;

    /* Extend computed flags */
    unsigned char *new_done = (unsigned char *)realloc(computed, (n + 1) * sizeof(unsigned char));
    if (!new_done)
        abort();
    computed = new_done;

    /* Mark new entries as not yet computed */
    for (idx_t i = max_cached + 1; i <= n; ++i)
        computed[i] = 0;

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of range, not used by OEIS */

    ensure_capacity(n);

    if (computed[n])
        return result_cache[n];

    uint32_t pn = prime_list[n];   /* p_n, the n‑th prime */
    seq_t   count = (seq_t)0;

    for (idx_t i = 1; i < n; ++i) {        /* primes p < p_n */
        uint32_t p = prime_list[i];
        seq_t prod = (seq_t)(p + 1) * (seq_t)(pn - 1);
        seq_t s = floor_sqrt(prod);
        if (s * s == prod)
            count++;
    }

    result_cache[n] = count;
    computed[n] = 1;
    return count;
}
