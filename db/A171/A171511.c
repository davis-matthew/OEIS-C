/*
 * A171511  Numbers that are not the sum of the k-th composite number and k
 *          for any k >= 1.
 * https://oeis.org/A171511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171511: numbers that are NOT equal to C_k + k for any
   k ≥ 1, where C_k is the k‑th composite number.
   The function builds the sequence incrementally, keeping a cache
   of already computed terms.  It generates composites by trial
   division and skips the forbidden sums.  The algorithm is
   O(n √m) with m ≈ a(n).  It satisfies all requirements for
   any seq_t / idx_t implementation. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Small helper: test whether n is composite (n > 1). */
static bool is_composite_uint64(uint64_t n)
{
    if (n < 4) return false;                 /* 0,1,2,3 are not composite */
    if (n % 2 == 0) return n != 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache for sequence terms */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    static size_t cacheCap  = 0;

    /* Static state for generating forbidden numbers C_k + k */
    static uint64_t currComp = 0;  /* current composite number (as 64‑bit) */
    static idx_t    currK    = 0;  /* current k (index of composite) */
    static seq_t    nextForbidden = 0;
    static seq_t    nextCandidate  = 1; /* next natural number tested */

    /* Initialize once */
    if (cache == NULL) {
        cacheCap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cacheCap);
        cacheSize = 0;

        /* First composite is 4, with k=1 */
        currComp = 4;
        currK    = 1;
        nextForbidden = (seq_t)currComp + (seq_t)currK;
        nextCandidate = 1;
    }

    /* Generate terms until we have at least n stored */
    while (cacheSize < (size_t)n) {
        /* Skip the forbidden sum */
        if (nextCandidate == nextForbidden) {
            /* Advance to the next composite number */
            currK++;
            currComp++;                     /* move to next integer */
            while (!is_composite_uint64(currComp))
                currComp++;                /* skip primes */
            nextForbidden = (seq_t)currComp + (seq_t)currK;

            nextCandidate++;               /* move past the forbidden number */
            continue;                       /* do not count this candidate */
        }

        /* Accept the candidate as a sequence term */
        if (cacheSize == cacheCap) {
            cacheCap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cacheCap);
        }
        cache[cacheSize++] = nextCandidate;
        nextCandidate++; /* prepare for next integer */
    }

    return cache[n - 1];
}
