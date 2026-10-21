/*
 * A070090  Number of scalene integer triangles with perimeter n and prime
 *          side lengths.
 * https://oeis.org/A070090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070090: Number of scalene integer triangles with perimeter n
 * and prime side lengths.
 * Count unordered triples of distinct primes p1 < p2 < p3 such
 * that p1 + p2 + p3 = n and p1 + p2 > p3.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool  *isPrime  = NULL;  /* sieve up to current maxN */
static int   *primes   = NULL;  /* list of primes up to current maxN */
static idx_t  maxN     = 0;     /* size of sieve / primes array */
static size_t primeCnt = 0;     /* number of primes <= maxN */

seq_t a(idx_t n)
{
    /* 0 and small n have zero triangles */
    if (n < 10) return (seq_t)0;

    /* rebuild sieve if necessary */
    if (n > maxN) {
        /* free old structures */
        if (isPrime) free(isPrime);
        if (primes)  free(primes);

        maxN = n;
        /* allocate sieve: cast for C++ */
        isPrime = (bool*) calloc((size_t)(maxN + 1), sizeof(bool));
        for (idx_t i = 2; i <= maxN; ++i) isPrime[i] = true;

        for (idx_t i = 2; i * i <= maxN; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= maxN; j += i)
                    isPrime[j] = false;
            }
        }

        /* estimate table size and allocate */
        size_t est = maxN > 2 ? maxN / 2 : 2;
        primes = (int*) malloc(est * sizeof(int));
        primeCnt = 0;

        for (idx_t i = 2; i <= maxN; ++i) {
            if (isPrime[i]) {
                if (primeCnt == est) {
                    est *= 2;
                    primes = (int*) realloc(primes, est * sizeof(int));
                }
                primes[primeCnt++] = (int)i;
            }
        }
    }

    seq_t count = 0;  /* result */

    /* enumerate unordered triples of distinct primes */
    for (size_t i = 0; i < primeCnt; ++i) {
        int p1 = primes[i];
        if ((idx_t)p1 >= n) break;          /* p1 alone already too large */

        for (size_t j = i + 1; j < primeCnt; ++j) {
            int p2 = primes[j];
            idx_t sum12 = (idx_t)p1 + (idx_t)p2;
            if (sum12 >= n) break;           /* p3 would be <= 0 */

            int p3 = (int)n - (int)sum12;
            if (p3 <= p2) continue;           /* enforce distinctness and order */

            if (p3 > (int)maxN) continue;      /* safety guard */
            if (!isPrime[p3]) continue;        /* p3 not prime */

            if (sum12 <= (idx_t)p3) continue;  /* triangle inequality */

            /* all conditions satisfied */
            ++count;
        }
    }

    return count;
}
