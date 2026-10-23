/*
 * A142640  Primes congruent to 54 mod 55.
 * https://oeis.org/A142640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 156, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th prime p with p ≡ 54 (mod 55). */
/* The implementation lazily generates candidates of the form 55k + 54
   and keeps those that are prime in a static array, the size of which
   is sufficient for the required range (n up to about 10000). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* maximum number of terms we expect to need */
#define MAX_CACHE  20000

/* static cache of primes of the form 55k+54 */
static seq_t prime_cache[MAX_CACHE];
static idx_t cache_size = 0;       /* number of primes stored so far */
static idx_t last_candidate = 54;  /* last number tested */

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* undefined, but harmless */
    }

    /* generate candidates until we have at least n primes cached */
    while (cache_size < n) {
        if (cache_size >= MAX_CACHE) {
            /* should not happen for the intended limits */
            return (seq_t)0;
        }

        /* next number congruent to 54 mod 55 */
        last_candidate += 55;
        if (is_prime_big((seq_t)last_candidate)) {
            prime_cache[cache_size++] = (seq_t)last_candidate;
        }
    }

    return prime_cache[n - 1];
}
