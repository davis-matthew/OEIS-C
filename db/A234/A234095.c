/*
 * A234095  Primes p such that 2*p + 1 is semiprime.
 * https://oeis.org/A234095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors, is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234095: primes p such that 2*p+1 is a product of two distinct primes. */
/* In OEIS notation: a(n) = the nth prime p with 2p+1 semiprime (distinct factors). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"
#include "../utils/count_prime_factors.h"

#define CACHE_CAP 10001   /* enough for n up to 10000; safety buffer */

static seq_t cache[CACHE_CAP];   /* stored sequence values */
static size_t cache_len = 0;     /* number of values currently stored */
static seq_t candidate = 3;      /* next odd number to test (prime or not) */

/* Test whether n is a product of two distinct primes. */
static bool is_semiprime_distinct(seq_t n)
{
    if (n <= 1)
        return false;          /* n must be >1 */
    if (!is_squarefree(n))
        return false;          /* must be square‑free to have distinct factors */
    return count_prime_factors(n) == (seq_t)2;
}

/* Append a new value to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len < CACHE_CAP)
        cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1; safe fallback */

    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate. */
        while (!is_prime_big(candidate))
            candidate += (seq_t)2;

        seq_t two_p_plus_one = candidate * (seq_t)2 + (seq_t)1;

        if (is_semiprime_distinct(two_p_plus_one))
            cache_append(candidate);

        candidate += (seq_t)2;   /* advance to the next odd number */
    }

    return cache[n - 1];
}
