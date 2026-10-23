/*
 * A153320  Primes p such that abs(p^2 - 48) and p^2 + 48 are also primes.
 * https://oeis.org/A153320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that |p² − 48| and p² + 48 are also
 *            primes
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;       /* cached sequence entries            */
static size_t cache_len = 0;      /* number of cached terms            */
static size_t cache_cap = 0;      /* allocated capacity of the cache   */
static idx_t last_checked = 2;    /* last integer examined for primality */

/* Ensure there is space for at least 'need' elements in the cache. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < need) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* Return the n‑th term (n≥1).  Undefined for n=0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds, return 0 */

    while (cache_len < (size_t)n) {
        /* Find the next prime after last_checked. */
        do {
            last_checked++;
        } while (!is_prime(last_checked));

        seq_t p = (seq_t)last_checked;

        /* Compute p², |p²−48| and p²+48 using a wide type. */
        unsigned long long sq   = (unsigned long long)p * (unsigned long long)p;
        unsigned long long diff = (sq >= 48ULL) ? (sq - 48ULL) : (48ULL - sq);
        unsigned long long plus = sq + 48ULL;

        /* Test the derived numbers for primality. */
        if (is_prime((idx_t)diff) && is_prime((idx_t)plus)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;   /* store the qualifying prime */
        }
    }
    return cache[n - 1];
}
