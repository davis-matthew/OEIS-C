/*
 * A045403  Primes congruent to {1, 3, 5} mod 8.
 * https://oeis.org/A045403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 4892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th prime p such that p ≡ 1, 3 or 5 (mod 8) via an incremental sieve. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Ensure a dynamic array of uint64_t has at least need elements. */
static void ensure_cap_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(**arr));
    *cap = new_cap;
}

/* Ensure a dynamic array of seq_t has at least need elements. */
static void ensure_cap_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(**arr));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache      = NULL; /* primes ≡ 1,3,5 mod 8 */
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    static uint64_t *prime_list = NULL;   /* list of all primes found */
    static size_t   prime_len   = 0;
    static size_t   prime_cap   = 0;
    static uint64_t next_candidate = 3;   /* odd numbers to test */

    /* initialise prime_list with 2 once */
    if (prime_len == 0) {
        ensure_cap_uint64(&prime_list, &prime_cap, 1);
        prime_list[prime_len++] = 2;
    }

    /* expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* primality test with trial division by known primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            /* stop if p*p > cand, using 128‑bit to avoid overflow */
            if ((__uint128_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store full prime list */
            ensure_cap_uint64(&prime_list, &prime_cap, prime_len + 1);
            prime_list[prime_len++] = cand;

            /* check congruence modulo 8 */
            if ((cand & 7ULL) != 7ULL) {          /* cand ≡ 1,3,5 (mod 8) */
                ensure_cap_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
            }
        }

        next_candidate += 2;  /* skip even numbers */
    }

    return cache[n - 1];
}
