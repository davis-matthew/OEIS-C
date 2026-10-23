/*
 * A171574  Primes of the form floor(binomial(k,2)/4).
 * https://oeis.org/A171574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 237, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p for which p = floor(binomial(k,2)/4)
   for some integer k ≥ 2.  The function generates the required
   primes lazily and caches them so that calls in arbitrary order
   remain efficient. */
#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool ----------------------- */
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n should be ≥ 1; if not, return 0 (undefined behaviour otherwise). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* static state for memoisation ------------------------------------- */
    static seq_t *primes = NULL;   /* array of accepted primes           */
    static size_t  cap   = 0;      /* capacity of the primes array       */
    static size_t  len   = 0;      /* number of primes stored so far     */
    static idx_t   k_next = 2;     /* next k to test (>=2)               */

    /* generate new primes until we have at least n of them */
    while (len < n) {
        /* value = floor(binomial(k_next,2)/4) = floor(k_next*(k_next-1)/8) */
        seq_t val = binomial(k_next, 2);
        val /= 4;                /* integer division is floor for positive operands */

        if (is_prime_big(val)) {           /* primality test for arbitrary precision */
            grow_seq(&primes, &cap, len + 1); /* ensure enough space            */
            primes[len++] = val;            /* store the prime                  */
        }
        ++k_next;
    }

    /* return the nth prime (1‑based index) */
    return primes[n - 1];
}
