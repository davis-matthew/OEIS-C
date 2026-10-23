/*
 * A142533  Primes congruent to 3 mod 53.
 * https://oeis.org/A142533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 3 modulo 53, computed by searching for
 *            primes
 * Range:     u8 n <= 2, u16 n <= 124, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static idx_t cached = 0;      /* number of terms already found */
static idx_t next_k = 0;      /* index of next candidate (3 + 53*next_k) */
#else
static seq_t *cache = NULL;
static idx_t cached = 0;      /* number of terms already found */
static size_t cap = 0;        /* allocated size of cache */
static idx_t next_k = 0;      /* index of next candidate (3 + 53*next_k) */
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset handling – unused, harmless */

    /* Fill cache until it contains at least n terms. */
    while (cached < n)
    {
        /* Candidate = 3 + 53 * next_k */
        seq_t cand = (seq_t)3 + (seq_t)53 * (seq_t)next_k;

        if (is_prime_big(cand))
        {
#ifdef __cplusplus
            cache.push_back(cand);
#else
            if (cached == cap)
            {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = realloc(cache, new_cap * sizeof(*cache));
                if (!tmp) { /* allocation failed – abandon */
                    free(cache);
                    cache = NULL;
                    cap = 0;
                    return 0;
                }
                cache = tmp;
                cap = new_cap;
            }
            cache[cached] = cand;
#endif
            ++cached;
        }
        ++next_k;
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
