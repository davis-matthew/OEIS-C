/*
 * A179295  a(n) is the least prime number such that prime(n)+a(n)+1 is a
 *          prime or -1 if no such prime number exists.
 * https://oeis.org/A179295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5980, u32 n <= 7321, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = least prime p such that prime(n)+p+1 is prime,
   or -1 if no such prime.  Uses get_prime, is_prime_big and
   ensure_cache to store results. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"    /* get_prime(idx_t k) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t n) */
#include "../utils/ensure_cache.h"/* ensure_cache(idx_t, seq_t**, size_t*) */

static seq_t *a_cache = NULL;            /* memoised results */
static size_t a_cache_cap = 0;           /* capacity of a_cache */

/* Return the nth value of A179295.  n is 1‑based.  */
seq_t a(idx_t n)
{
    /* nothing is defined for n==0; return 0 to avoid UB */
    if (n == 0) return 0;

    /* enlarge cache if necessary */
    if (n > a_cache_cap) {
        ensure_cache(n, &a_cache, &a_cache_cap);
    }

    /* return cached value if we already computed it */
    if (a_cache[n - 1] != 0) {
        return a_cache[n - 1];
    }

    /* prime(n) is the nth prime */
    seq_t prime_n = get_prime(n);

    /* iterate over primes in increasing order */
    idx_t p_idx = 1;              /* 1‑based index into primes */
    seq_t result = 0;             /* will hold the found prime */

    while (1) {
        seq_t p = get_prime(p_idx);
        seq_t sum = prime_n + p + (seq_t)1;

        if (is_prime_big(sum)) {
            result = p;
            break;
        }
        /* Safety guard: avoid infinite loop if no prime found */
        /* In practice a solution exists for all tested n. */
        /* We still provide fallback after a large search limit. */
        if (p_idx > 1000000) {
            result = (seq_t)-1;      /* indicates no solution found */
            break;
        }
        ++p_idx;
    }

    /* cache the result for future calls */
    a_cache[n - 1] = result;
    return result;
}
