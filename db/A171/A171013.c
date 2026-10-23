/*
 * A171013  In the sequence of prime numbers, replace all digits '1' with '0'
 *          and vice versa.
 * https://oeis.org/A171013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform( p_n ) where p_n is the nth prime and
 *            `transform` replaces every decimal digit 1 by 0 and every digit
 *            0 by 1 (other digits unchanged)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_realloc, transform_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"
#include "../utils/transform_prime.h"

/* Cached list of primes (as seq_t). */
static seq_t *prime_cache = NULL;      /* array of primes */
static size_t  prime_cap   = 0;        /* allocated capacity          */
static size_t  prime_cnt   = 0;        /* number of primes stored     */
static uint64_t candidate  = 2;        /* next integer to test for primality */

/* Ensure the cache contains at least `n` primes. */
static void
ensure_primes(idx_t n)
{
    while (prime_cnt < n)
    {
        if (is_prime_small((seq_t)candidate))
        {
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = seq_realloc(prime_cache, prime_cap, new_cap);
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = (seq_t)candidate;
        }
        candidate++;                /* advance to next candidate */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the OEIS offset. */
    ensure_primes(n);
    seq_t p = prime_cache[n - 1];
    return transform_prime(p);
}
