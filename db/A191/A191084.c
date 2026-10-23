/*
 * A191084  Primes p that have Kronecker symbol (p|87) = -1.
 * https://oeis.org/A191084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with Jacobi symbol (p|87) = -1 */
/* Generates terms on demand; works for any order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/jacobi_symbol.h"

/* ---------- cache for the sequence -------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ---------- cache for prime list (for trial division) ---- */
static uint64_t *prime_list = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* ---------- next candidate prime --------------------- */
static uint64_t candidate = 3;    /* start searching from the first odd prime */

/* ---------- local helper: add a new sequence term --------*/
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);            /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- local helper: add a new prime to list ------*/
static void add_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap == 0 ? 64 : primes_cap * 2;
        uint64_t *new_primes = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
        if (!new_primes) exit(1);
        if (prime_list) {
            memcpy(new_primes, prime_list, primes_len * sizeof(uint64_t));
            free(prime_list);
        }
        prime_list = new_primes;
        primes_cap = new_cap;
    }
    prime_list[primes_len++] = p;
}

/* ---------- ensure at least the twin prime 2 is in list ------*/
static bool prime_list_initialized = false;
static void init_prime_list(void)
{
    if (prime_list_initialized) return;
    add_prime(2);
    prime_list_initialized = true;
}

/* ---------- simple primality test by trial division ---------*/
static bool is_prime_candidate(uint64_t p)
{
    if (p < 2) return false;
    if (p == 2) return true;
    init_prime_list();

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t q = prime_list[i];
        /* use q*q to avoid overflow (q <= sqrt(p) is harmless) */
        if (q * q > p) break;
        if (p % q == 0) return false;
    }
    return true;
}

/* ---------- generate terms up to index n (inclusive) --------*/
static void generate_to(idx_t n)
{
    init_prime_list();   /* ensure 2 is in prime_list */

    while (cache_len < (size_t)n) {
        /* Find next prime */
        while (!is_prime_candidate(candidate))
            candidate += 2;   /* skip even numbers */

        /* prime found: add to prime list */
        add_prime(candidate);

        /* skip primes dividing 87 (3 and 29) */
        if (candidate == 3 || candidate == 29) {
            candidate += 2;
            continue;
        }

        /* Jacobi symbol test */
        int js = jacobi_symbol(candidate, 87);
        if (js == -1)
            add_term((seq_t)candidate);

        candidate += 2;   /* move to next odd number */
    }
}

/* ---------- public entry point ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, but safe */
    if (cache_len < (size_t)n)
        generate_to(n);
    return cache[n - 1];
}
