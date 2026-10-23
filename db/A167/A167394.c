/*
 * A167394  Largest single or isolated prime < n-th single or isolated
 *          composite.
 * https://oeis.org/A167394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest single or isolated prime < (n-th single or
 *            isolated composite)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A single or isolated composite is a number m such that m-1 and m+1 are prime;
   these are precisely the middle of twin‑prime pairs. */
/* A single or isolated prime is a prime p that is not part of a twin‑prime pair
   (i.e., p-2 and p+2 are not prime). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const idx_t INITIAL_LIMIT = 4000000u;   /* enough for n up to ~22k twin primes */

static void build_tables(idx_t limit, uint8_t **pf, idx_t **twin, size_t *twin_cnt,
                         size_t *twin_cap, idx_t **iso,  size_t *iso_cnt,
                         size_t *iso_cap)
{
    /* Sieve of Eratosthenes: prime flag array */
    *pf = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!*pf) return;
    for (size_t i = 0; i <= (size_t)limit; ++i) (*pf)[i] = 1;
    (*pf)[0] = (*pf)[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p)
        if ((*pf)[p])
            for (idx_t j = p * p; j <= limit; j += p)
                (*pf)[j] = 0;

    /* Allocate storage for twin primes and isolated primes */
    *twin_cap = 30000;          /* initial capacity, will grow if needed */
    *twin = (idx_t*)malloc((*twin_cap) * sizeof(idx_t));
    *twin_cnt = 0;

    *iso_cap = 250000;
    *iso = (idx_t*)malloc((*iso_cap) * sizeof(idx_t));
    *iso_cnt = 0;

    /* Scan all primes to fill the tables */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!(*pf)[p]) continue;

        /* Isolated prime? */
        bool isolated = false;
        if (p == 2) {
            isolated = true;
        } else {
            bool left_composite  = !(*pf)[p - 2];
            bool right_composite = (p + 2 > limit) ? true : !(*pf)[p + 2];
            isolated = left_composite && right_composite;
        }
        if (isolated) {
            if (*iso_cnt == *iso_cap) {
                *iso_cap *= 2;
                *iso = (idx_t*)realloc(*iso, (*iso_cap) * sizeof(idx_t));
            }
            (*iso)[(*iso_cnt)++] = p;
        }

        /* Twin prime pair (p, p+2) */
        if (p + 2 <= limit && (*pf)[p + 2]) {
            if (*twin_cnt == *twin_cap) {
                *twin_cap *= 2;
                *twin = (idx_t*)realloc(*twin, (*twin_cap) * sizeof(idx_t));
            }
            (*twin)[(*twin_cnt)++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t LIMIT = 0;
    static uint8_t *prime_flag = NULL;
    static idx_t *twinPrimes = NULL;
    static size_t twinCount = 0, twinCap = 0;
    static idx_t *isolatedPrimes = NULL;
    static size_t isoCount = 0, isoCap = 0;

    if (!initialized)
    {
        LIMIT = INITIAL_LIMIT;
        build_tables(LIMIT, &prime_flag, &twinPrimes, &twinCount, &twinCap,
                     &isolatedPrimes, &isoCount, &isoCap);
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined for n=0, but safe */

    /* Ensure we have enough twin primes for this n */
    idx_t idx_n = n - 1;                   /* 0‑based index */
    if (idx_n >= twinCount)
    {
        /* Double the limit until we have enough twin primes */
        while (idx_n >= twinCount)
        {
            LIMIT *= 2;
            free(prime_flag);  free(twinPrimes);  free(isolatedPrimes);

            build_tables(LIMIT, &prime_flag, &twinPrimes, &twinCount, &twinCap,
                         &isolatedPrimes, &isoCount, &isoCap);
        }
    }

    /* n‑th isolated composite = twinPrimes[idx_n] + 1 */
    idx_t p = twinPrimes[idx_n];
    idx_t target = p + 1;                 /* composite, even, >2 */

    /* Binary search for largest isolated prime < target */
    size_t lo = 0, hi = isoCount;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (isolatedPrimes[mid] >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t pos = lo;                      /* first isolated prime >= target */
    seq_t result = 0;
    if (pos > 0) result = (seq_t)isolatedPrimes[pos - 1];

    return result;
}
