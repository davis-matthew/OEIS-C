/*
 * A087878  Primes of the form (4n+3)^2 + (4m)^2, m,n = 0,1,2..
 * https://oeis.org/A087878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   prime_has_representation, uint64_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// Compute the nth prime of the form (4a+3)^2 + (4b)^2  (OEIS A087878)
// The sequence starts with a(1)=73.  n is 1‑based; n>=1.
//
// The algorithm is a simple sieve up to an increasing bound until enough
// primes of the desired form have been found.  For each prime p we test
// whether p = (4a+3)^2 + (4b)^2 by iterating a and checking if the
// remainder is a multiple of 16 and a perfect square.  The test is
// efficient for the required range (n <= 10000 in the tests).
//
// Only standard C libraries are used, and no global state is required.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/prime_has_representation.h"
#include "../utils/uint64_sqrt.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Estimate a reasonable starting limit for the sieve  */
    uint64_t limit = (uint64_t)(n + 10) * 200;          /* rough overshoot */
    if (limit < 1000U) limit = 1000U;                   /* minimum size */

    while (1) {
        /* ---------- sieve ---- */
        size_t sz = (size_t)(limit + 1);
        unsigned char *isprime = (unsigned char*)malloc(sz);
        if (!isprime) return 0;                  /* allocation failure */

        /* initialize all as prime candidates */
        for (size_t i = 0; i < sz; ++i) isprime[i] = 1;
        if (sz > 0) isprime[0] = 0;
        if (sz > 1) isprime[1] = 0;

        /* simple Eratosthenes sieve */
        for (uint64_t i = 2; i*i <= limit; ++i) {
            if (isprime[(size_t)i]) {
                for (uint64_t j = i*i; j <= limit; j += i)
                    isprime[(size_t)j] = 0;
            }
        }

        /* ---------- collect required primes ---- */
        idx_t collected = 0;
        seq_t result = 0;

        for (size_t p = 2; p <= limit && collected < n; ++p) {
            if (!isprime[(size_t)p]) continue;
            if (prime_has_representation((seq_t)p)) {
                ++collected;
                if (collected == n) {
                    result = (seq_t)p;
                    break;
                }
            }
        }

        free(isprime);

        if (collected >= n) {
            return result;                      /* found the nth term */
        }

        /* not enough yet – grow the sieve limit and try again */
        limit <<= 1;                            /* double the bound */
    }
}
