/*
 * A139238  First differences of Mersenne numbers A001348.
 * https://oeis.org/A139238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 2^prime(n+1) - 2^prime(n) (OEIS A139238)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided pow2() and build_primes() utilities. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow2.h"
#include "../utils/build_primes.h"

/* Cache the list of primes up to a pre‑chosen limit.
   The limit is set high enough for the typical test range (n ≤ 10000).
   If a larger n is requested, the function will simply use the
   pre‑computed primes (which is sufficient, since the prime number
   theorem guarantees p_n < n log n for n ≥ 6). */
static bool initialized = false;
static idx_t *primes   = NULL;   /* array of primes, 0‑indexed */
static size_t prime_cnt = 0;     /* number of primes stored */

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* initialise the prime cache once */
    if (!initialized) {
        /* For n up to about 10000, the 10001‑st prime is < 200000.
           We take a generous limit to avoid re‑sieving. */
        idx_t limit = 200000;          /* adjust if higher n is needed */
        build_primes(limit, &primes, &prime_cnt);
        initialized = true;
    }

    /* If the cached primes are not enough for n+1, extend the cache. */
    if (prime_cnt <= (size_t)n) {
        /* Grow the sieve limit until we have at least n+1 primes. */
        idx_t limit = primes && prime_cnt > 0 ? (primes[prime_cnt - 1] * 2) : 100;
        while (prime_cnt <= (size_t)n) {
            /* Free the old array to avoid memory leak. */
            free(primes);
            limit = limit * 2;
            build_primes(limit, &primes, &prime_cnt);
        }
    }

    /* Compute 2^(prime(n+1)) - 2^(prime(n)) */
    idx_t pn   = primes[n - 1];       /* prime(n)  */
    idx_t pn1  = primes[n];           /* prime(n+1) */

    seq_t p2n  = pow2(pn);
    seq_t p2n1 = pow2(pn1);

    return p2n1 - p2n;
}
