/*
 * A142299  Primes congruent to 17 mod 44.
 * https://oeis.org/A142299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 334, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142299: primes ≡ 17 (mod 44)
   (computed by searching for the next prime of the form 44·k+17) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t *prime_cache   = NULL;   /* array of found primes (seq_t) */
static size_t cache_capacity = 0;     /* current allocated capacity */
static idx_t cache_size     = 0;      /* number of primes stored */
static idx_t next_candidate = 0;     /* next residue‑17 candidate to test */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive; not used for OEIS offset 1 */

    /* Grow the cache until we have at least n primes. */
    if (cache_size < n) {
        /* Allocate or expand the storage. */
        size_t new_cap = cache_capacity == 0 ? 64 : cache_capacity;
        while (new_cap < (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        prime_cache = tmp;
        cache_capacity = new_cap;

        /* Initialize the first prime if the cache has just been created. */
        if (cache_size == 0) {
            prime_cache[0] = (seq_t)17;
            cache_size = 1;
            next_candidate = 61;  /* 17 + 44 */
        }

        /* Search for further primes of the form 44k + 17. */
        while (cache_size < n) {
            idx_t cand = next_candidate;
            /* Skip non‑prime candidates. */
            while (!is_prime(cand))
                cand += 44;
            prime_cache[(size_t)cache_size] = (seq_t)cand;
            cache_size++;
            next_candidate = cand + 44;
        }
    }

    return prime_cache[(size_t)(n - 1)];
}
