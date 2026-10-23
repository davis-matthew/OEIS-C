/*
 * A124387  Largest prime < 2*a(n-1) written in binary, a(1)=2.
 * https://oeis.org/A124387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < 2*a(n-1), with a(1)=2. The function
 *            returns that prime written in binary, interpreted as a decimal
 *            number (i.e. the string of 0/1 digits)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *val_cache   = NULL;   /* cached outputs (binary <‑> decimal)    */
static seq_t *prime_cache = NULL;   /* cached primes (actual prime values)   */
static size_t cache_len  = 0;       /* number of cached terms              */
static size_t cache_cap  = 0;       /* allocated capacity                   */

static void ensure_cache(idx_t n)
{
    if (n == 0) return;                       /* safety guard */
    size_t target = (size_t)n;

    /* grow caches if needed */
    if (cache_cap < target) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < target) new_cap <<= 1;
        seq_t *new_val   = seq_alloc_new_array(new_cap);
        seq_t *new_prime = seq_alloc_new_array(new_cap);
        if (cache_len > 0) {
            for (size_t k = 0; k < cache_len; ++k) {
                new_val[k]   = val_cache[k];
                new_prime[k] = prime_cache[k];
            }
            seq_free(val_cache);
            seq_free(prime_cache);
        }
        val_cache   = new_val;
        prime_cache = new_prime;
        cache_cap   = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < target; ++i) {
        seq_t cand;
        if (i == 0) {
            cand = (seq_t)2;                /* a(1) prime */
        } else {
            seq_t prev_prime = prime_cache[i - 1];
            seq_t twice = prev_prime + prev_prime;
            cand = twice - (seq_t)1;        /* start below 2*a(n-1) */
            while (!is_prime_big(cand))     /* find largest prime smaller */
                cand = cand - (seq_t)1;
        }

        /* convert prime to decimal number whose digits are its bits */
        seq_t val = 0;
        seq_t temp = cand;
        unsigned bitlen = 0;
        while (temp > 0) {                  /* count bits */
            temp >>= 1;
            ++bitlen;
        }
        for (int j = (int)bitlen - 1; j >= 0; --j) {
            val = val * (seq_t)10 + ((cand >> j) & (seq_t)1);
        }

        val_cache[i]   = val;
        prime_cache[i] = cand;
    }
    cache_len = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0 in OEIS */
    ensure_cache(n);
    return val_cache[(size_t)n - 1];
}
