/*
 * A135734  a(n) = prime(n)*prime(n+1)*prime(n+2) - prime(n+3)^2.
 * https://oeis.org/A135734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 10, i32 n <= 208, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135734
   a(n) = prime(n) * prime(n+1) * prime(n+2) – prime(n+3)^2
   Offset: 1
*/

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;   /* stored primes, 1‑based index = array index+1   */
static idx_t   prime_cached = 0;       /* number of primes currently cached            */

/* Ensure that at least 'needed' primes are cached.
   The sieve_first_n_primes function allocates an array of that size.
   On each call we free any previous allocation.                   */
static void
ensure_primes(idx_t needed)
{
    if (prime_cached >= needed) return;      /* already have enough      */

    free(prime_cache);                        /* discard old allocation   */
    sieve_first_n_primes(needed, &prime_cache, &prime_cached);
}

seq_t
a(idx_t n)
{
    /* Sequence is 1‑based; a(1) uses the first 3 primes and prime(4).   */
    if (n == (idx_t)-1) return 0;            /* defensive guard */

    idx_t needed = n + 3;                    /* need primes up to prime(n+3) */

    ensure_primes(needed);

    /* Retrieve the required primes (array is 0‑based).               */
    seq_t p1 = (seq_t)prime_cache[n-1];      /* prime(n)       */
    seq_t p2 = (seq_t)prime_cache[n];        /* prime(n+1)     */
    seq_t p3 = (seq_t)prime_cache[n+1];      /* prime(n+2)     */
    seq_t p4 = (seq_t)prime_cache[n+2];      /* prime(n+3)     */

    /* Compute product first, then subtract square of p4.              */
    seq_t prod = p1 * p2;
    prod *= p3;
    seq_t sub  = p4 * p4;
    return prod - sub;
}
