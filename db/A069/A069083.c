/*
 * A069083  Numbers n such that sigma(n)/tau(n) has denominator 4.
 * https://oeis.org/A069083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th positive integer m such that the reduced
 *            denominator of σ(m)/τ(m) equals 4, where σ is the sum of
 *            divisors and τ is the divisor count. Equivalently, τ(m) = 4 *
 *            gcd(σ(m), τ(m))
 * Range:     u8 n <= 5, u16 n <= 391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

static seq_t *cache      = NULL;   /* stored sequence values          */
static size_t cache_len  = 0;      /* number of terms cached          */
static size_t cache_cap  = 0;      /* capacity of cache array         */
static uint64_t next_candidate = 1;/* next integer to test             */

/* Find the next integer that satisfies the condition and
   advance next_candidate. */
static seq_t
find_next(void)
{
    for (;;)
    {
        size_t tau   = divisor_count_u64(next_candidate);
        seq_t  sigma = sigma_of(next_candidate);
        seq_t  g     = gcd(sigma, (seq_t)tau);

        if ((seq_t)tau == (seq_t)4 * g) {
            seq_t val = (seq_t)next_candidate;
            ++next_candidate;
            return val;
        }
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = find_next();
    }
    return cache[(size_t)n - 1];
}
