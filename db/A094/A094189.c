/*
 * A094189  Number of primes between n^2-n and n^2 (inclusive).
 * https://oeis.org/A094189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3843, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094189: number of primes between n^2-n and n^2 (inclusive) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Shared table of primes ≤ current limit used for segmented sieving */
static uint64_t *small_primes = NULL;   /* primes array */
static size_t   small_cnt   = 0;        /* number of primes */
static idx_t    small_limit = 0;        /* current maximum prime in table */

/* Ensure the table contains all primes ≤ limit (limit ≥ 2).  Rebuilds
   the table if necessary. */
static void
ensure_small_primes(idx_t limit)
{
    if (small_primes != NULL && small_limit >= limit)
        return;                       /* already covers the needed range */

    /* free the old table, if any */
    if (small_primes != NULL) {
        free(small_primes);
        small_primes = NULL;
    }

    /* BF.  The build function expects a non‑zero limit.  For limit==1 we
       simply create an empty array. */
    if (limit >= 2) {
        build_primes_upto(limit, &small_primes, &small_cnt);
        small_limit = limit;
    } else {
        small_primes = NULL;
        small_cnt    = 0;
        small_limit  = 0;
    }
}

/* Count primes in the closed interval [low, high] using a segmented sieve
   over the precomputed small primes.  Assumes low ≤ high and that
   all needed primes for sieving are present in small_primes.  */
static seq_t
count_primes_in_interval(uint64_t low, uint64_t high)
{
    /* Length of the interval is high - low + 1.  For the target
       interval it is exactly n+1, but we compute the general case. */
    const size_t len = (size_t)(high - low + 1);
    unsigned char *segment = (unsigned char *)calloc(len, 1);  /* 0 = potentially prime */
    if (segment == NULL)
        return 0;  /* out of memory: return 0 (unlikely for the expected sizes) */

    /* Mark composites in the segment */
    for (size_t i = 0; i < small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > high)
            break;   /* beyond sqrt(high) */

        /* First multiple of p within [low, high] */
        uint64_t start = (low + p - 1) / p * p;
        if (start < p)
            start = p;   /* ensure we do not mark the prime itself */

        for (uint64_t x = start; x <= high; x += p) {
            if (x == p)
                continue;  /* keep the prime */
            segment[(size_t)(x - low)] = 1;  /* mark composite */
        }
    }

    /* Count primes in the interval */
    size_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        uint64_t val = low + (uint64_t)i;
        if (val >= 2 && segment[i] == 0)
            ++count;
    }

    free(segment);
    return (seq_t)count;
}

/* Main sequence function.  External linkage is required. */
seq_t
a(idx_t n)
{
    /* By definition a(1) = 0, and the formula works for all n ≥ 2. */
    if (n <= 1)
        return 0;

    /* Compute the interval [low, high] = [n^2 - n, n^2] */
    uint64_t nn   = (uint64_t)n * (uint64_t)n;  /* n^2 */
    uint64_t low  = nn - (uint64_t)n;           /* n^2 - n */
    uint64_t high = nn;                         /* n^2 */

    /* Make sure small primes are available up to n (since sqrt(high)=n). */
    if (small_limit < (idx_t)n)
        ensure_small_primes((idx_t)n);

    /* Count primes in the interval */
    return count_primes_in_interval(low, high);
}
