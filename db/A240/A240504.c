/*
 * A240504  Read (exponents of primes in the factorization of n!) modulo 2 and
 *          convert to decimal.
 * https://oeis.org/A240504
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = read the exponents of primes in the factorisation of n!
 *            modulo 2, treating the resulting binary digits (smallest prime
 *            first) as a base-2 integer
 * Range:     u8 n <= 22, u16 n <= 61, u32 n <= 137, u64 n <= 313,
 *            u128 n <= 727, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Static cache for primes up to the largest request so far. */
static uint64_t *prime_cache = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

/* Ensure that primes up to n are available in prime_cache. */
static void ensure_primes(idx_t n)
{
    if (prime_limit >= n)
        return;
    uint64_t *new_cache = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(n, &new_cache, &new_cnt);
    free(prime_cache);
    prime_cache = new_cache;
    prime_cnt   = new_cnt;
    prime_limit = n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* value not defined below offset */

    ensure_primes(n);

    seq_t result = 0;              /* accumulate binary digits */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > n)               /* primes are sorted */
            break;

        /* Compute exponent of p in n! modulo 2. */
        idx_t term = n / p;          /* first floor(n/p) */
        int   parity = 0;            /* 0 or 1 */

        while (term) {
            parity ^= (int)(term & 1); /* toggle if term is odd */
            term /= p;
        }

        /* Append bit to the left: small primes are more significant. */
        result = (result << 1) | (seq_t)(parity & 1);
    }

    return result;
}
