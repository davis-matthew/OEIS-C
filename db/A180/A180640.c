/*
 * A180640  Primes P such that P < (largest prime factor of (P-1)) * (largest
 *          prime factor of (P+1)).
 * https://oeis.org/A180640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A180640
   a(n) = the nth prime P such that P < LPF(P-1) * LPF(P+1)
   (LPF = largest prime factor) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"               /* bool is_prime(idx_t) */
#include "../utils/largest_prime_factor.h"   /* uint64_t largest_prime_factor(uint64_t) */

/* static cache of qualifying primes, 1‑based indexing */
static seq_t *prime_cache = NULL;
static size_t cache_len  = 0;          /* number of stored terms */
static size_t cache_cap  = 0;          /* allocated capacity */
static uint64_t next_candidate = 2;    /* next integer to test */

/* grow the cache when needed */
static void grow_cache(void)
{
    cache_cap = cache_cap ? cache_cap * 2 : 64;
    prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    if (!prime_cache) abort();
}

/* ensures that cache_len >= need.  If not, it continues searching
   for qualifying primes until the required number of terms is found. */
static void build_cache(idx_t need)
{
    if (cache_len >= (size_t)need) return;

    while (cache_len < (size_t)need) {
        /* find the next qualifying prime */
        while (1) {
            if (is_prime(next_candidate)) {
                uint64_t lf1 = largest_prime_factor(next_candidate - 1);
                uint64_t lf2 = largest_prime_factor(next_candidate + 1);
                /* candidate < lf1 * lf2 ⇔ candidate / lf2 < lf1          */
                if (next_candidate % lf2 == 0 ? (next_candidate / lf2) < lf1
                                              : (next_candidate / lf2) < lf1) {
                    /* store the prime */
                    if (cache_len >= cache_cap) grow_cache();
                    prime_cache[cache_len++] = (seq_t)next_candidate;
                    next_candidate++;          /* advance for subsequent search */
                    break;                     /* move on to next list entry */
                }
            }
            next_candidate++;
        }
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 */
    build_cache(n);
    return prime_cache[(size_t)n - 1];   /* 1‑based index */
}
