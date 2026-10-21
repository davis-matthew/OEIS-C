/*
 * A060687  Numbers k such that there exist exactly 2 Abelian groups of order
 *          k, i.e., A000688(k) = 2.
 * https://oeis.org/A060687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq_array, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A060687: numbers k such that exactly one
   prime in the prime factorisation of k has exponent 2
   and all other primes have exponent 1.
   The sequence is generated on demand by checking successive
   integers; the results are cached so that any order of calls
   works efficiently. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/grow_seq_array.h"     /* grow_seq_array() */
#include "../utils/int_sqrt.h"          /* int_sqrt() */

/* Internal test: does k have exactly one prime squared? */
static bool single_square_prime(uint64_t k)
{
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t primes_lim = 0;   /* largest prime included in primes[] */

    /* Ensure we have primes up to sqrt(k) */
    uint64_t lim = (uint64_t)int_sqrt(k) + 1;
    if (lim > primes_lim) {
        build_primes_upto(lim, &primes, &primes_cnt);
        primes_lim = lim;
    }

    size_t sq_cnt = 0;
    uint64_t t = k;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > t) break;
        if (t % p == 0) {
            size_t exp = 0;
            do {
                t /= p;
                ++exp;
            } while (t % p == 0);
            if (exp == 2) ++sq_cnt;
            else if (exp > 2) return false;
        }
    }
    /* Remaining t is 1 or a prime > sqrt(k) */
    return sq_cnt == 1;
}

/* Cache for generated terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t checked_to = 3;   /* last integer examined, start at 3 so next is 4 */

/* Ensure that we have at least n terms stored in cache */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* Grow capacity if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* Find new terms until we reach n */
    while (cache_len < (size_t)n) {
        ++checked_to;
        if (single_square_prime(checked_to)) {
            cache[cache_len++] = (seq_t)checked_to;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 to avoid UB */
    ensure_cache(n);
    return cache[n - 1];
}
