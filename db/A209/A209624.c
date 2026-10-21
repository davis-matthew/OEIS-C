/*
 * A209624  Primes separated from their previous adjacent primes by a prime
 *          number of successive composites.
 * https://oeis.org/A209624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that the number of composites between p *
 *            and its preceding prime q equals a prime number, i.e. (p - q -
 *            1) is * prime. Returns the nth term of OEIS A209624
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/*  seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /*  Static cache of already found terms.  */
    static seq_t *terms   = NULL;
    static size_t  len    = 0;
    static size_t  cap    = 0;
    static uint64_t prev   = 2;      /* previous prime inspected   */
    static uint64_t current = 2;     /* last prime returned by next_prime */

    /*  Initialise on first call.  */
    if (len == 0) {
        cap   = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /*  Keep generating primes until we have at least n terms stored.  */
    while (len < (size_t)n) {
        /*  Next prime after the previous one.  */
        current = next_prime(prev);

        /*  Number of composites between the two primes.  */
        uint64_t gap = current - prev;
        if (gap > 1 && is_prime_big((seq_t)(gap - 1))) {
            /*  Ensure capacity.  */
            if (len >= cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)current;   /* store as seq_t */
        }

        prev = current;   /* move to the prime we just processed   */
    }

    /*  Return the requested term (indices are 1‑based).  */
    return terms[(size_t)(n - 1)];
}
