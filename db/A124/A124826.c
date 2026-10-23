/*
 * A124826  Primes congruent to 1 mod 21.
 * https://oeis.org/A124826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 535, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth prime congruent to 1 modulo 21. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static cache of found primes */
static seq_t *prime_cache   = NULL;   /* primes found so far */
static size_t cache_cap     = 0;      /* allocated capacity         */
static size_t cache_size    = 0;      /* number of primes stored    */
static idx_t  candidate     = 43;     /* next candidate ≡ 1 (mod 21) */
static bool   initialized   = false;

/* initialise static state */
static void init(void)
{
    if (!initialized) {
        prime_cache = NULL;
        cache_cap   = 0;
        cache_size  = 0;
        candidate   = 43;
        initialized = true;
    }
}

/* ensure capacity for at least `need` elements.  Works for C and C++. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* abort on allocation failure */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

/* grow the cache until it contains at least `n` primes. */
static void generate_to(idx_t n)
{
    while (cache_size < n) {
        if (is_prime(candidate)) {
            ensure_capacity(cache_size + 1);
            prime_cache[cache_size++] = (seq_t)candidate;
        }
        candidate += 21;                /* next number ≡ 1 (mod 21) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, safe fallback  */
    init();
    generate_to(n);
    return prime_cache[n - 1];          /* zero‑based array index            */
}
