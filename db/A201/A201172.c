/*
 * A201172  Primes of the form 5n^3+2.
 * https://oeis.org/A201172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 56, u64 n <= 1700,
 *            u128 n <= 1700, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime of the form 5*k^3+2 (OEIS A201172). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *prime_cache = NULL;   /* array of found primes            */
static idx_t  cache_size  = 0;       /* number of primes stored          */
static idx_t  cache_cap   = 0;       /* allocated capacity in elements    */
static idx_t  last_k      = 0;       /* last candidate k processed       */

/* Ensure that the cache can hold at least 'needed' primes.   */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1U : 64U;
    while (new_cap < needed)
        new_cap <<= 1U;

    seq_t *tmp;
    if (cache_cap == 0) {
        tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        tmp = (seq_t*)realloc(prime_cache, new_cap * sizeof(seq_t));
    }
    if (!tmp)
        abort();
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Append a newly found prime to the cache. */
static void append_prime(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    prime_cache[cache_size++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n == 0 */

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return prime_cache[n - 1];

    /* Determine the next candidate k to test. */
    idx_t k = (cache_size == 0) ? 0 : last_k + 1;

    while (cache_size < n) {
        seq_t val;
        if (k == 0) {                         /* first candidate: 2 */
            val = (seq_t)2;
        } else {
            /* Even k > 0 produce even numbers, skip. */
            if ((k & 1U) == 0) {
                ++k;
                continue;
            }
            seq_t kk = (seq_t)k;
            val = (seq_t)5 * kk * kk * kk + (seq_t)2;
        }

        if (isPrimeGeneric(val))
            append_prime(val);

        last_k = k;
        ++k;
    }

    return prime_cache[n - 1];
}
