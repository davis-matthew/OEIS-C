/*
 * A131378  Starting with 0, the sequence a(n) changes from 0 to 1 or back
 *          when the next number n is a prime.
 * https://oeis.org/A131378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) XOR 1 if n is prime, else a(n-1); a(0)=0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache of values a(0)..a(current_max) */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* list of primes discovered so far */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 4;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) return;           /* allocation failure – ignore, caller will hit a cache miss */
    prime_list = tmp;
    prime_cap = new_cap;
}

/* trial‑division using the stored primes */
static bool is_prime_idx(idx_t v)
{
    if (v < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > v / p) break;   /* p*p > v */
        if (v % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (a_cache == NULL) {
        a_cache_size = 2;                                 /* store a(0) and a(1) */
        a_cache = (seq_t *)calloc(a_cache_size, sizeof(seq_t));
        if (!a_cache) return (seq_t)0;                   /* allocation failure */
        a_cache[0] = 0;
        a_cache[1] = 0;
    }

    if (prime_list == NULL) {
        prime_cap = 4;
        prime_list = (idx_t *)calloc(prime_cap, sizeof(idx_t));
        if (!prime_list) return (seq_t)0;
        prime_count = 0;
    }

    /* If value already cached, return it */
    if (n < a_cache_size) return a_cache[n];

    /* Expand a_cache to store up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                 /* allocation failure */
    a_cache = tmp;

    /* Compute all values from the current size up to n */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        bool prime = is_prime_idx(i);
        if (prime) {
            if (prime_count == prime_cap) grow_prime_list();
            prime_list[prime_count++] = i;
        }
        /* XOR with 1 if prime, otherwise keep previous value */
        a_cache[i] = a_cache[i-1] ^ (seq_t)(prime ? 1 : 0);
    }
    a_cache_size = new_size;

    return a_cache[n];
}
