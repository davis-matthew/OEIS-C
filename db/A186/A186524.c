/*
 * A186524  Numbers n such that n == prime(n) (mod 7).
 * https://oeis.org/A186524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n ≡ prime(n) (mod 7)
   Computation: sequential search using a dynamic sieve of primes.
   The nth term a(n) is the nth integer m for which
   m mod 7 equals the m‑th prime (prime(m)) mod 7.             */

#include <stdlib.h>
#include <stdint.h>

/* ----------  Prime sieve utilities  ---------- */
static uint32_t *prime_list = NULL;   /* array of primes (1‑based)          */
static size_t    prime_cnt  = 0;      /* number of primes currently stored  */
static uint8_t  *comp_arr   = NULL;   /* temporary composite marker          */
static size_t    sieve_lim  = 0;      /* current sieve limit                 */
static size_t    prime_cap  = 0;      /* allocated capacity for prime_list   */

/* Rebuild the sieve up to new_limit, storing all primes ≤ new_limit. */
static void recompute_primes(size_t new_limit)
{
    if (comp_arr) free(comp_arr);
    comp_arr = (uint8_t*)calloc(new_limit + 1, 1);
    if (!comp_arr) abort();            /* out‑of‑memory */

    if (prime_list) free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_cap  = 0;

    for (size_t i = 2; i <= new_limit; ++i) {
        if (!comp_arr[i]) {
            /* grow prime array if needed */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_cnt++] = (uint32_t)i;

            if ((uint64_t)i * i <= new_limit) {
                for (size_t j = i * i; j <= new_limit; j += i) comp_arr[j] = 1;
            }
        }
    }

    sieve_lim = new_limit;
}

/* Ensure that at least `need` primes are available (i.e. prime_cnt >= need). */
static void ensure_prime_capacity(size_t need)
{
    while (prime_cnt < need) {
        size_t new_limit = sieve_lim ? sieve_lim * 2 : 10;
        if (new_limit < need * 15) new_limit = need * 15;   /* crude upper bound on p_n */
        recompute_primes(new_limit);
    }
}

/* ----------  Sequence implementation  ---------- */
static seq_t *cache      = NULL;   /* cached terms of the sequence          */
static idx_t  cache_len  = 0;      /* number of terms currently cached      */
static idx_t  cache_cap  = 0;      /* allocated capacity for cache          */
static idx_t  next_candidate = 1;  /* next integer to test for membership   */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n ≤ 0 */

    /* Grow the sequence cache until it contains n terms. */
    while (cache_len < n) {
        /* Ensure prime list has at least next_candidate primes. */
        ensure_prime_capacity((size_t)next_candidate);

        /* Test whether current candidate satisfies the congruence. */
        if ((next_candidate % 7) == (prime_list[(size_t)next_candidate - 1] % 7)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 256;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];          /* zero‑based indexing into the cache */
}
