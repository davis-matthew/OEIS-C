/*
 * A162250  Values of the form prime(prime(i)) with a prime digital sum.
 * https://oeis.org/A162250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 309, u32 n <= 359, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th prime of the form prime(prime(i))
 *            whose decimal digit sum is also prime.
 * Algorithm: generate primes on demand, test each prime(prime(i))
 *            for the digit‑sum condition, and memoize the valid terms.
 *            Each call to a(n) works independently of call order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/digit_sum.h"     /* digit_sum(seq_t) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) */

/* ---------- Prime table ---------------------------------------- */

static idx_t  *prime_table = NULL;     /* prime_table[pos-1] = pos‑th prime */
static size_t prime_cap   = 0;
static size_t prime_len   = 0;
static idx_t  prime_next  = 2;        /* next candidate to test */

/* Ensure the prime table can hold 'new_cap' entries. */
static void grow_prime_table(size_t new_cap)
{
    if (prime_cap >= new_cap)
        return;
    idx_t *tmp =
        (idx_t *)realloc(prime_table, new_cap * sizeof(idx_t));
    if (!tmp && new_cap > 0)
        exit(EXIT_FAILURE);
    prime_table = tmp;
    prime_cap   = new_cap;
}

/* Make sure at least 'needed' primes are available in the table. */
static void ensure_primes_upto_index(idx_t needed)
{
    while (prime_len < needed) {
        /* Find next prime. */
        while (!is_prime_big((seq_t)prime_next))
            ++prime_next;
        grow_prime_table(prime_len + 1);
        prime_table[prime_len++] = prime_next;
        ++prime_next;
    }
}

/* ---------- Sequence memoization -------------------------------- */

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_i     = 1;      /* 1‑based index i for prime(i) */

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp && new_cap > 0)
            exit(EXIT_FAILURE);
        cache      = tmp;
        cache_cap  = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function --------------------------------------- */

seq_t a(idx_t n)
{
    /* Build up to the n‑th qualifying term. */
    while (cache_len < n) {
        /* prime(i) */
        ensure_primes_upto_index(next_i);
        idx_t prime_i = prime_table[next_i - 1];

        /* prime(prime(i)) */
        ensure_primes_upto_index(prime_i);
        seq_t candidate = prime_table[prime_i - 1];

        /* digit sum check */
        seq_t sum = digit_sum(candidate);
        if (is_prime_big(sum))
            cache_append(candidate);

        ++next_i;
    }
    return cache[n - 1];
}
