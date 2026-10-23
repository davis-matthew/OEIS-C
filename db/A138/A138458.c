/*
 * A138458  a(n) = ((n-th prime)^6-(n-th prime)^4)/24.
 * https://oeis.org/A138458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n))^6-(prime(n))^4)/24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* Cache for the first primes computed so far. */
static uint32_t *primes_cache = NULL;
static size_t primes_cached = 0;

/* Return the n-th term of OEIS sequence A138458.  The index n starts
   at 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined but harmless */

    /* Ensure we have at least n primes cached. */
    if (primes_cached < (size_t)n) {
        free(primes_cache);
        size_t cnt;
        sieve_first_n_primes((idx_t)n, &primes_cache, &cnt);
        primes_cached = cnt;
    }

    /* primes_cache is 0‑based, so nth prime is at index n-1. */
    uint32_t p = primes_cache[n - 1];
    seq_t ps = (seq_t)p;

    seq_t p2 = ps * ps;     /* p^2 */
    seq_t p4 = p2 * p2;     /* p^4 */
    seq_t p6 = p4 * p2;     /* p^6 */

    seq_t result = (p6 - p4) / (seq_t)24;
    return result;
}
