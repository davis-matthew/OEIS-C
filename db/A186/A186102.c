/*
 * A186102  Smallest prime p such that p == n (mod prime(n)).
 * https://oeis.org/A186102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 332, u32 n <= 1763, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, upper_bound_for_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = smallest prime p such that p ≡ n (mod prime(n)) */

#include <stdint.h>     /* for uint32_t, uint64_t, etc. */
#include <stdlib.h>     /* for calloc, realloc, free */
#include <math.h>       /* for log */
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t n) */
#include "../utils/upper_bound_for_nth_prime.h"

/* static cache for primes */
static uint64_t *prime_cache = NULL;   /* array of primes (1‑based indexing in logic) */
static size_t prime_cache_len = 0;     /* number of primes cached */
static uint64_t sieve_limit = 0;       /* largest index sieved */

/* extend the prime cache so that it contains at least 'needed' primes */
static void ensure_prime_cache(idx_t needed) {
    if (prime_cache_len >= (size_t)needed) return;

    uint64_t new_limit = upper_bound_for_nth_prime(needed);
    if (new_limit < sieve_limit) new_limit = sieve_limit;

    /* allocate a boolean sieve (0 = prime, 1 = composite) */
    uint8_t *is_comp = (uint8_t *)calloc(new_limit + 1, 1);
    if (!is_comp) return;  /* allocation failed – leave cache unchanged */

    is_comp[0] = is_comp[1] = 1;
    uint64_t i;
    for (i = 2; i * i <= new_limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* count primes up to new_limit */
    size_t count = 0;
    for (i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) ++count;

    /* realloc prime cache */
    uint64_t *new_cache = (uint64_t *)realloc(prime_cache, count * sizeof(uint64_t));
    if (!new_cache) {
        free(is_comp);
        return; /* allocation failed – leave cache unchanged */
    }
    prime_cache = new_cache;
    prime_cache_len = count;
    sieve_limit = new_limit;

    /* fill the cache */
    size_t idx = 0;
    for (i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) prime_cache[idx++] = i;

    free(is_comp);
}

/* return the n‑th prime (1‑based), n ≥ 1 */
static inline uint64_t nth_prime(idx_t n) {
    if (n == 0) return 0;           /* defensive: offset is 1 but handle zero */
    ensure_prime_cache(n);
    return prime_cache[n - 1];
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety, should never be called */

    uint64_t p_n = nth_prime(n);          /* prime(n) */
    seq_t r = (seq_t)n % (seq_t)p_n;     /* remainder n mod prime(n) */

    /* the smallest prime p with p % prime(n) == r */
    seq_t candidate;
    if (r == (seq_t)0)                    /* only possible prime is prime(n) itself */
        candidate = (seq_t)p_n;
    else
        candidate = r;                    /* start with the remainder itself */

    /* search forward in steps of prime(n) until a prime is found */
    while (!is_prime_big(candidate)) {
        candidate += (seq_t)p_n;
    }
    return candidate;
}
