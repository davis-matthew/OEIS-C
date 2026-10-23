/*
 * A019354  Primes with primitive root 28.
 * https://oeis.org/A019354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates primes p for which 28 is a primitive root modulo p. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* helpers ------------------------------------------------------------ */
static void get_prime_factors_u64(uint64_t m, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    if (m % 2ULL == 0ULL) {
        factors[(*cnt)++] = 2ULL;
        while (m % 2ULL == 0ULL) m /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= m; d += 2ULL) {
        if (m % d == 0ULL) {
            factors[(*cnt)++] = d;
            while (m % d == 0ULL) m /= d;
        }
    }
    if (m > 1ULL) factors[(*cnt)++] = m;
}

/* true if 28 is a primitive root modulo the odd prime p */
static bool prime_has_primitive_root_28(uint64_t p)
{
    /* 28 is not coprime with 2 or 7, so these primes cannot work */
    if (p == 2ULL || p == 7ULL) return false;

    uint64_t factors[64];
    size_t cnt;
    get_prime_factors_u64(p - 1ULL, factors, &cnt);

    seq_t base = (seq_t)28;
    seq_t mod  = (seq_t)p;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = factors[i];
        if (mod_pow(base, (uint64_t)((p - 1ULL) / q), mod) == (seq_t)1) {
            return false;           /* order divides (p-1)/q */
        }
    }
    return true;
}

/* cache -------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 5;    /* start scanning from 5 */

/* ----------------------------------------------------------------------------
 *  a(idx_t n) - Return n-th prime for which 28 is a primitive root.
 *  The sequence is 1-indexed.  The function is memoised so calls can
 *  arrive in any order.
 * --------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1 */

    /* initialise cache */
    if (!cache) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }

    /* extend cache until it contains n terms */
    while (cache_len < (size_t)n) {
        /* make sure we only test odd numbers > 3 */
        if (next_candidate <= 3) next_candidate = 5;

        if (is_prime_small((seq_t)next_candidate)) {
            if (prime_has_primitive_root_28(next_candidate)) {
                /* grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap + cache_cap / 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        /* skip even numbers */
        next_candidate += 2;
    }

    return cache[n - 1];
}
