/*
 * A158244  The number of consecutive prime pairs with 6-gap in n-th century.
 * https://oeis.org/A158244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive prime pairs with difference 6 that
 *            lie entirely within the n‑th century
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve of Eratosthenes up to the required limit and then counts the 6‑gap prime pairs per century. */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------------- */
/* static storage                                                           */
/* ------------------------------------------------------------------------- */
static uint64_t *primeList   = NULL;   /* array of primes up to current sieveLimit           */
static size_t   primeCount   = 0;      /* number of primes stored                          */
static idx_t    sieveLimit   = 0;      /* upper bound that has been sieved                 */

static seq_t   *centuryCount = NULL;   /* count of 6‑gap prime pairs per century (1‑based) */
static idx_t    maxCentury   = 0;      /* number of centuries whose counts are stored       */

/* ------------------------------------------------------------------------- */
/* Recompute a sieve up to at least 'limit' (inclusive).                     */
/* The sieve is built as a simple Eratosthenes treatment.                    */
/* ------------------------------------------------------------------------- */
static void recompute_sieve(idx_t limit)
{
    /* allocate array of prime flags; calloc zero‑initialises */
    unsigned char *isPrime = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    if (!isPrime) exit(EXIT_FAILURE);

    /* mark primes (1 means prime, 0 means composite) */
    for (idx_t i = 2; i <= limit; ++i) isPrime[i] = 1;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                isPrime[j] = 0;
            }
        }
    }

    /* build list of primes from the flag array */
    size_t primeCap = (size_t)(limit / 10 + 10);
    uint64_t *plist = (uint64_t*)malloc(primeCap * sizeof(uint64_t));
    if (!plist) exit(EXIT_FAILURE);
    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (isPrime[i]) {
            if (pcnt == primeCap) { /* grow the prime array if needed */
                primeCap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(plist, primeCap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                plist = tmp;
            }
            plist[pcnt++] = (uint64_t)i;
        }
    }

    /* clean up old data and store new lists */
    free(primeList);
    primeList   = plist;
    primeCount  = pcnt;
    sieveLimit  = limit;
    free(isPrime);
}

/* ------------------------------------------------------------------------- */
/* Recompute the 6‑gap prime‑pair counts for centuries up to 'centuries'.   */
/* The array is 1‑based: centuryCount[1] holds the count for the first     */
/* century (1–100), centuryCount[2] for 101–200, etc.                        */
/* ------------------------------------------------------------------------- */
static void recompute_century_counts(idx_t centuries)
{
    seq_t *tmp = (seq_t*)calloc(centuries + 1, sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    for (size_t i = 0; i + 1 < primeCount; ++i) {
        uint64_t p = primeList[i];
        uint64_t q = primeList[i + 1];
        if (q - p == 6) {
            idx_t century = ((idx_t)(p - 1) / 100ULL) + 1;          /* century containing the lower prime */
            if (q <= (uint64_t)century * 100ULL) {                /* both primes are inside the same century */
                ++tmp[century];
            }
        }
    }

    free(centuryCount);
    centuryCount = tmp;
    maxCentury   = centuries;
}

/* ------------------------------------------------------------------------- */
/* Public sequence function                                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset */

    idx_t neededLimit = n * 100ULL;       /* highest number needed for the requested century */

    if (sieveLimit < neededLimit)       /* extend sieve if necessary */
        recompute_sieve(neededLimit);

    if (maxCentury < n)                 /* extend century counts if necessary */
        recompute_century_counts(n);

    return centuryCount[n];              /* return the cached result */
}
