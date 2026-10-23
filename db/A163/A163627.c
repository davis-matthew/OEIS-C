/*
 * A163627  Numbers k such that 42k + 5 is prime.
 * https://oeis.org/A163627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 677, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163627 – numbers k such that 42*k + 5 is prime.
   This implementation searches k sequentially and caches the values.
   The function works correctly for all n ≥ 1 that fit in seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cache of found k values (externally represented as seq_t). */
static seq_t *k_cache      = NULL; /* array of discovered k values   */
static size_t k_cache_cap  = 0;    /* current allocation capacity   */
static size_t k_cache_cnt  = 0;    /* number of valid entries        */
static idx_t  next_k       = 0;    /* next k to test                 */

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= k_cache_cap) return;
    size_t new_cap = k_cache_cap ? k_cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* memory allocation failure */
    k_cache = tmp;
    k_cache_cap = new_cap;
}

/* Return the n-th non‑negative integer k with 42*k + 5 prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* OEIS offset is 1 – undefined for n=0 */
    while (k_cache_cnt < n) {
        /* Compute 42*next_k + 5 in idx_t to avoid early truncation. */
        idx_t val_i = 42 * next_k + 5;
        seq_t val   = (seq_t)val_i;       /* cast to seq_t for primality test */
        if (is_prime_big(val)) {
            ensure_cache_cap(k_cache_cnt + 1);
            k_cache[k_cache_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return k_cache[n - 1];
}
