/*
 * A064712  Numbers k such that k^2 - prime(k) is a prime.
 * https://oeis.org/A064712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064712: numbers k such that k^2 - prime(k) is prime */
/* Computation: brute‑force using get_prime() and is_prime_big() */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k);      */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n);       */
#include "../utils/alloc_seq_array.h"  /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"   /* void free_seq_array(seq_t *p);    */

static seq_t *cache       = NULL;   /* cached sequence values       */
static size_t cache_len   = 0;      /* number of terms already cached */
static size_t cache_cap   = 0;      /* current capacity of cache     */
static idx_t  tested_k    = 1;      /* next candidate to test       */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache == NULL) {
        cache_cap = 128;
        cache     = alloc_seq_array(cache_cap);
    }
    while (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp      = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Compute the sequence up to at least index n. */
static void compute_up_to(idx_t n)
{
    if (n == 0) return;
    seq_t one = 1;
    while (cache_len < (size_t)n) {
        seq_t pk  = (seq_t)get_prime(tested_k);            /* prime_k   */
        seq_t val = (seq_t)tested_k * (seq_t)tested_k - pk;/* k^2 - pk  */
        if (val > one && is_prime_big(val)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)tested_k;
        }
        ++tested_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n = 0 – avoid UB */
    compute_up_to(n);
    return cache[n - 1];
}
