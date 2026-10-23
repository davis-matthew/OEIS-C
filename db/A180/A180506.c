/*
 * A180506  The largest palindromic prime dividing n, or 0 if such a prime
 *          does not exist.
 * https://oeis.org/A180506
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest palindromic prime divisor of n, or 0 if none
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   build_primes_upto, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_palindrome.h"

/* Static cache of primes up to the current limit. */
static uint64_t *primes_cache = NULL;
static size_t    primes_cnt_cache = 0;
static idx_t     primes_limit_cache = 0;

/* Ensure primes are available up to at least 'limit'. */
static void ensure_primes(idx_t limit)
{
    if (primes_cache != NULL && primes_limit_cache >= limit)
        return;

    /* Free previous cache if any and rebuild. */
    if (primes_cache != NULL) {
        free(primes_cache);
        primes_cache = NULL;
    }

    build_primes_upto(limit, &primes_cache, &primes_cnt_cache);
    primes_limit_cache = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) /* outside OEIS offset */
        return 0;

    uint64_t temp = (uint64_t)n;
    /* At least the square‑root of n is needed for factorisation. */
    /* Building up to n is simpler and fast for n <= 10000. */
    ensure_primes(n);

    seq_t best = 0;

    for (size_t i = 0; i < primes_cnt_cache; ++i) {
        uint64_t p = primes_cache[i];
        /* Stop when p > sqrt(temp). Using division avoids overflow. */
        if (p > temp / p)
            break;

        if (temp % p == 0) {
            /* Repeatedly divide out all copies of p. */
            do {
                temp /= p;
            } while (temp % p == 0);

            if (is_palindrome((seq_t)p, 10) && (seq_t)p > best)
                best = (seq_t)p;
        }
    }

    /* If a prime factor > sqrt(original n) remains. */
    if (temp > 1) {
        /* temp itself is prime. */
        if (is_palindrome((seq_t)temp, 10) && (seq_t)temp > best)
            best = (seq_t)temp;
    }

    return best;
}
