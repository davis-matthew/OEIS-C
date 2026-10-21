/*
 * A206768  a(n) = smallest number k such that sigma(k-n) = sigma(k) - n, with
 *          k > n+1.
 * https://oeis.org/A206768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   _build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SEARCH:  a(n) = smallest k > n+1 such that sigma(k-n) = sigma(k) – n,
   where sigma(m) is the sum of all positive divisors of m.
   Computed by brute‑force search with an on‑the‑fly prime sieve for
   factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_build_primes.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Sum of divisors function via prime factorisation.
   n is cast to seq_t; primes are uint32_t; the calculation is
   performed using seq_t arithmetic. */
static seq_t _sigma_of(seq_t n,
                       const uint32_t *primes, size_t primes_cnt)
{
    if (n <= 1) return n;         /* sigma(0)=0, sigma(1)=1 */

    seq_t result = 1;
    seq_t temp   = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > temp) break;

        if (temp % p == 0) {
            seq_t sum   = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum += power;
            }
            result *= sum;
        }
    }

    if (temp > 1) result *= (1 + temp);   /* remaining prime factor */
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  It employs a static cache so that
   repeated calls for the same n avoid recomputation. */
seq_t a(idx_t n)
{
    /* Cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static uint32_t *primes  = NULL;
    static size_t primes_cnt = 0;
    static bool primes_built  = false;

    /* Build the prime array once */
    if (!primes_built) {
        _build_primes(&primes, &primes_cnt);
        primes_built = true;
    }

    /* Resize cache if needed */
    if (n >= cache_size) {
        size_t new_size = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        /* initialise new entries to zero (uncomputed) */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache      = new_cache;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    seq_t n_seq = n;
    seq_t start = (seq_t)(n + 2);        /* smallest admissible k */

    for (seq_t k = start; ; ++k) {
        seq_t sub   = k - n_seq;
        seq_t sigma_k   = _sigma_of(k, primes, primes_cnt);
        seq_t sigma_sub = _sigma_of(sub, primes, primes_cnt);

        if (sigma_k - sigma_sub == n_seq) {
            cache[n] = k;   /* cache for future */
            return k;
        }
    }
}
