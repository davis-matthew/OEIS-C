/*
 * A229271  Numbers k for which k + k' and k - k' are both prime, k' being the
 *          arithmetic derivative of k.
 * https://oeis.org/A229271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 655, u32 n <= 872, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k for which k + k' and k – k' are both prime
   with k' the arithmetic derivative of k.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t next_candidate = 1; /* smallest integer not yet examined */

/* Compute the arithmetic derivative of a uint64_t number.
   For n = ∏ p_i^e_i  ->  n' = Σ (n / p_i) * e_i */
static uint64_t derivative_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t res = 0;
    uint64_t nn = n;
    /* factor 2 separately */
    if ((nn & 1ULL) == 0) {
        unsigned int e = 0;
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
            ++e;
        }
        res += (n >> 1) * e;          /* n/2 * e */
    }
    /* factor odd primes */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            unsigned int e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            res += (n / p) * e;
        }
    }
    /* remaining prime factor */
    if (nn > 1) {
        res += (n / nn);
    }
    return res;
}

/* Ensure the cache has at least n terms (n >=1).  */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Search for the next qualifying k */
        for (uint64_t k = next_candidate + 1; ; ++k) {
            uint64_t d = derivative_uint64(k);
            if (k <= d) continue; /* k - d would not be positive prime */

            unsigned __int128 sum = (unsigned __int128)k + d;
            unsigned __int128 diff = (unsigned __int128)k - d;

            seq_t x1 = (seq_t)sum;
            seq_t x2 = (seq_t)diff;

            if (is_prime_big(x1) && is_prime_big(x2)) {
                /* store the term */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)k;
                next_candidate = k;
                break;
            }
        }
    }
}

/* Return the n-th term (1‑based) of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
