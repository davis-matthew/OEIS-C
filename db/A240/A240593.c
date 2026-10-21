/*
 * A240593  The smaller of a pair of consecutive composite squarefree numbers
 *          (A120944) without any prime number between them.
 * https://oeis.org/A240593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term a(n) of OEIS A240593.
   a(n) is the smaller of a pair of consecutive composite squarefree numbers
   with no primes between them.  The function uses a prime sieve up to a
   fixed maximim (3 000 000) and generates the sequence on demand.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fixed upper limit for the sieve – large enough for n ≤ 10 000. */
    static const uint64_t LIMIT = 3000000;

    /* One‑time initialization of global data. */
    static bool initialized = false;
    static uint8_t *isPrime = NULL;
    static uint8_t *isSquarefree = NULL;
    static uint32_t *primePrefix = NULL;

    /* Cache of computed terms. */
    static seq_t *terms = NULL;
    static size_t termsLen = 0, termsCap = 0;

    /* State for incremental generation. */
    static uint64_t nextIdx = 2;            /* next integer to test   */
    static uint64_t prevCsf = 0;            /* previous composite sf */
    static bool hasPrev = false;            /* whether prevCsf set    */

    /* One‑time sieve construction. */
    if (!initialized) {
        isPrime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        isSquarefree = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        primePrefix = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));

        memset(isPrime, 1, (LIMIT + 1) * sizeof(uint8_t));
        memset(isSquarefree, 1, (LIMIT + 1) * sizeof(uint8_t));
        isPrime[0] = isPrime[1] = 0;

        /* Classical sieve of Eratosthenes. */
        for (uint64_t i = 2; i * i <= LIMIT; ++i) {
            if (isPrime[i]) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    isPrime[j] = 0;
            }
        }

        /* Mark numbers containing a square factor. */
        for (uint64_t p = 2; p * p <= LIMIT; ++p) {
            if (isPrime[p]) {
                uint64_t sq = p * p;
                for (uint64_t j = sq; j <= LIMIT; j += sq)
                    isSquarefree[j] = 0;
            }
        }

        /* Prefix sum of prime counts. */
        primePrefix[0] = 0;
        for (uint64_t i = 1; i <= LIMIT; ++i)
            primePrefix[i] = primePrefix[i - 1] + (isPrime[i] ? 1 : 0);

        /* Prepare term cache. */
        termsCap = 128;
        terms = (seq_t *)malloc(termsCap * sizeof(seq_t));

        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (termsLen < (size_t)n && nextIdx <= LIMIT) {
        uint64_t i = nextIdx++;

        /* Check if i is a composite squarefree number. */
        if (!isPrime[i] && isSquarefree[i]) {
            if (hasPrev) {
                /* No primes strictly between prevCsf and i? */
                if (primePrefix[i - 1] - primePrefix[prevCsf] == 0) {
                    /* Store the smaller member of the qualifying pair. */
                    if (termsLen == termsCap) {
                        termsCap <<= 1;
                        terms = (seq_t *)realloc(terms, termsCap * sizeof(seq_t));
                    }
                    terms[termsLen++] = (seq_t)prevCsf;
                }
            }
            prevCsf = i;
            hasPrev = true;
        }
    }

    /* If n is 1 or higher and terms were generated, return. */
    if (n >= 1 && termsLen >= (size_t)n)
        return terms[(size_t)n - 1];

    /* Fallback: if n is out of precomputed range (should not happen for n ≤ 10 000). */
    return (seq_t)0;
}
