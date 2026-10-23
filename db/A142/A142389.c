/*
 * A142389  Primes congruent to 38 mod 47.
 * https://oeis.org/A142389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 38 (mod 47)
 * Range:     u8 n <= 1, u16 n <= 150, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

static seq_t *prime_cache = NULL;   /* array of found primes   */
static idx_t  prime_cache_len = 0;  /* number of primes cached */
static idx_t  prime_cache_cap = 0;  /* current allocated capacity */

static idx_t next_k = 3;            /* counter for 47*k+38, start with k=3 (179) */

/* Allocate a larger array and copy existing primes into it. */
static void grow_prime_cache(idx_t needed)
{
    if (prime_cache_cap < needed) {
        idx_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
        while (new_cap < needed) new_cap <<= 1;
        /* alloc_seq_array constructs seq_t objects appropriately. */
        seq_t *tmp = alloc_seq_array(new_cap);
        if (!tmp) exit(1);            /* allocation failed */
        for (idx_t i = 0; i < prime_cache_len; ++i)
            tmp[i] = prime_cache[i];  /* copy assignment */
        free_seq(prime_cache);        /* destroy old array */
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
}

/* Ensure we have at least n primes in the cache. */
static void ensure_primes(idx_t n)
{
    while (prime_cache_len < n) {
        seq_t cand = ((seq_t)47 * (seq_t)next_k) + (seq_t)38;
        if (is_prime_big(cand)) {
            grow_prime_cache(prime_cache_len + 1);
            prime_cache[prime_cache_len++] = cand;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1; safe fallback */
    ensure_primes(n);
    return prime_cache[n - 1];
}
