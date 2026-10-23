/*
 * A142526  Primes congruent to 41 mod 52.
 * https://oeis.org/A142526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A142526 – primes congruent to 41 mod 52 */
/* a(n) = the nth prime satisfying p ≡ 41 (mod 52) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides   bool is_prime(idx_t x); */

static seq_t *prime_cache = NULL;   /* cache of consecutive terms */
static idx_t  cache_len   = 0;      /* number of terms currently cached */
static size_t cache_cap   = 0;      /* allocated capacity of the cache */
static idx_t  next_candidate = 41;  /* next integer to test (≡41 mod 52) */

static void ensure_cache_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
#if defined(__cplusplus)
        prime_cache = new seq_t[cache_cap];
#else
        prime_cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
#endif
    } else if (cache_len >= cache_cap) {
        cache_cap <<= 1;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[cache_cap];
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = prime_cache[i];
        delete [] prime_cache;
        prime_cache = tmp;
#else
        prime_cache = (seq_t*)realloc(prime_cache, cache_cap * sizeof(seq_t));
#endif
    }
}

/* Return the nth term (n ≥ 1).  The function is order‑invariant
 * and memoises all results. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* undefined for offset 0 */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            ensure_cache_capacity();
            prime_cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 52u;    /* stay in the residue class 41 mod 52 */
    }
    return prime_cache[n - 1];
}
