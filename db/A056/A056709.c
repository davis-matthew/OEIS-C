/*
 * A056709  Naught-y primes, primes with noughts (or zeros).
 * https://oeis.org/A056709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1688, u32 n <= 1004, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: This function searches for primes that contain at least
 * one decimal zero digit (the A056709 “naught‑y primes”).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Forward declaration of has_zero_digit */
static inline bool has_zero_digit(seq_t n)
{
    /* Zero digit check: iterate over decimal digits */
    while (n > 0) {
        if ((n % 10) == 0)
            return true;
        n /= 10;
    }
    return false;
}

/* Static cache for computed terms */
static seq_t  *cache      = NULL;        /* array of seq_t values */
static idx_t   cached     = 0;           /* number of stored terms   */
static size_t  capacity   = 0;           /* current capacity of cache */
static seq_t  last_checked = 0;          /* highest integer examined so far */

/* Ensure cache holds at least `n` elements (resize if necessary) */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;
    if (capacity == 0)
        capacity = 4;
    while (capacity < n)
        capacity <<= 1;                 /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    if (!tmp)
        abort();                        /* out of memory */
    cache = tmp;
}

/* Sequence accessor: a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined offset – graceful */

    ensure_cache(n);

    /* Generate candidates until we have enough terms */
    seq_t candidate = last_checked + 1;
    while (cached < n) {
        if (is_prime_big(candidate) && has_zero_digit(candidate))
            cache[cached++] = candidate;
        ++candidate;
    }
    last_checked = candidate - 1;
    return cache[n - 1];
}
