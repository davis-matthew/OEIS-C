/*
 * A124374  Primes of the form !(k + 1)/2 = Sum_{i=0..k} i!/2.
 * https://oeis.org/A124374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of (∑_{i=0}^k i!)/2 where k ≥ 2, i.e. a(n) is
 *            the nth value of S(k)/2 that is prime
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Static state to allow O(1) access to previously computed terms. */
static seq_t *primes = NULL;      /* array of previously found primes   */
static size_t primes_len = 0;      /* number of primes stored          */
static size_t primes_cap = 0;      /* current capacity of primes array  */
static idx_t   last_k  = 0;        /* largest k processed so far       */
static seq_t   fact    = 1;        /* fact  = k!  for current k        */
static seq_t   sum     = 1;        /* sum   = Σ_{i=0}^k i! for current k*/

/* Return the nth term of A124374, 1‑based.  When n is 0 or negative the
   function returns 0.  The function is resilient to calls in any order
   and grows its cache lazily until it contains at least n primes. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range indices */

    /* If we already know the nth prime, return it immediately. */
    if (primes_len >= (size_t)n)
        return primes[(size_t)n - 1];

    /* Continue generating primes until we reach the required index. */
    for (idx_t k = last_k + 1; ; ++k) {
        /* Update factorial: fact = k! */
        fact = fact * (seq_t)k;
        /* Update sum: sum = Σ_{i=0}^k i! */
        sum  = sum  + fact;

        if (k >= 2) {                   /* primes only start at k = 2  */
            seq_t candidate = sum / 2;
            if (is_prime_big(candidate)) {
                /* Grow the cache if necessary. */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *new_primes =
                        seq_realloc(primes, primes_cap, new_cap);
                    primes  = new_primes;
                    primes_cap = new_cap;
                }
                /* Store the prime and check whether we obtained n of them. */
                primes[primes_len++] = candidate;
                if (primes_len >= (size_t)n) {
                    last_k = k;                /* remember last processed k */
                    return primes[(size_t)n - 1];
                }
            }
        }
    }
}
