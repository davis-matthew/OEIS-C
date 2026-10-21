/*
 * A057131  One less than six times product of first n primes of form 6k-1.
 * https://oeis.org/A057131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * (Π_{i=1}^{n} p_i) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   next_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_i are the first n primes of the form 6k-1. */
/* Implementation uses memoization and incremental prime generation. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* 1‑indexed array of computed terms   */
static idx_t cache_capacity = 0;     /* allocated capacity (in terms)      */
static idx_t cache_len = 0;          /* highest index already computed     */

static seq_t prod = (seq_t)1;        /* product of the primes processed   */
static uint64_t last_prime_f6 = 0;   /* last prime of the form 6k‑1      */

/* Return the next prime of the form 6k-1, updating last_prime_f6. */
static uint64_t next_prime_form6k1(void)
{
    if (last_prime_f6 == 0) {
        last_prime_f6 = 5;           /* first 6k‑1 prime */
        return last_prime_f6;
    }
    uint64_t cand = next_prime(last_prime_f6);
    while (cand % 6 != 5) {
        cand = next_prime(cand);
    }
    last_prime_f6 = cand;
    return cand;
}

/* Grow the cache to accommodate at least 'needed' indices. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_capacity)
        return;
    idx_t new_cap = (cache_capacity == 0) ? 1 : cache_capacity;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = seq_alloc_new_array(new_cap + 1);  /* 1‑indexed */
    if (cache) {
        for (idx_t i = 1; i <= cache_len; ++i)
            new_arr[i] = cache[i];
        seq_free_array(cache);
    }
    cache = new_arr;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; value for 0 is unused */
        return (seq_t)0;
    if (n <= cache_len)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        uint64_t p = next_prime_form6k1();          /* next 6k‑1 prime */
        prod *= (seq_t)p;                           /* update product   */
        seq_t val = (seq_t)6 * prod - (seq_t)1;     /* a(i) = 6*prod - 1 */
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
