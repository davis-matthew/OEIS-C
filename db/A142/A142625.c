/*
 * A142625  Primes congruent to 34 mod 55.
 * https://oeis.org/A142625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 166, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A142625:
   a(n) = the nth prime p such that p ≡ 34 (mod 55)   (offset 1)
   The implementation generates primes of the form 55*k+34
   on demand, storing them in a static array to support
   calls in any order.  Each candidate is tested with the
   generic primality routine is_prime_big. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint64_t *primes = NULL;   /* primes ≡ 34 (mod 55) */
    static size_t prime_count = 0;    /* number of primes stored */
    static size_t prime_cap   = 0;    /* array capacity */
    static idx_t   next_k      = 0;    /* next k to try in 55*k+34 */

    /* Grow the array when needed (non‑class types). */
    if (prime_count == prime_cap)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        primes     = tmp;
        prime_cap  = new_cap;
    }

    /* Generate primes until we have at least n of them. */
    while (prime_count < (size_t)n)
    {
        uint64_t candidate = 55ULL * next_k + 34ULL;
        if (is_prime_big((seq_t)candidate))
        {
            primes[prime_count++] = candidate;
        }
        ++next_k;
    }

    /* Return the requested prime (offset 1). */
    return (seq_t)primes[n - 1];
}
