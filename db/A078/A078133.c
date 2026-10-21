/*
 * A078133  Primes which cannot be written as sum of cubes>1.
 * https://oeis.org/A078133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes that cannot be expressed as a sum of cubes > 1.
   The sequence is finite; this implementation generates all primes
   up to a safe bound, checks each for representability by a
   dynamic programming coin‑change routine with the allowed cubes,
   and stores the unsummable primes. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Static storage for the computed sequence. */
static uint64_t *unsummable_primes = NULL; /* array of primes that fail */
static size_t      unsummable_count = 0;

/* Initialise the sequence once, lazily. */
static void
init_sequence(void)
{
    if (unsummable_primes != NULL)      /* already computed */
        return;

    /* A safe upper bound; the last element is 127, so 10,000 is ample. */
    const idx_t LIMIT = 10000;

    /* 1. Generate all primes up to LIMIT. */
    uint64_t *primes_u64 = NULL;
    size_t   prime_cnt   = 0;
    build_primes_upto(LIMIT, &primes_u64, &prime_cnt);
    if (primes_u64 == NULL)
        return;    /* safety: no primes */

    /* 2. Build the list of cubes i^3 (i >= 2) that are <= LIMIT. */
    idx_t cubes[200];    /* 200 cubes > 2^3 are already > 10,000 */
    size_t cube_cnt = 0;
    for (idx_t i = 2;; ++i) {
        seq_t cube = (seq_t)i * i * i;  /* i^3 */
        if (cube > LIMIT)
            break;
        cubes[cube_cnt++] = (idx_t)cube;
    }

    /* 3. DP: dp[s] = true if s can be expressed as sum of the cubes. */
    bool *dp = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (dp == NULL) {
        free(primes_u64);
        return;      /* allocation failure */
    }
    for (size_t i = 0; i <= LIMIT; ++i)
        dp[i] = false;
    dp[0] = true;

    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        const idx_t c = cubes[ci];
        for (size_t s = c; s <= LIMIT; ++s)
            if (dp[s - c])
                dp[s] = true;
    }

    /* 4. Collect primes that are not representable. */
    uint64_t *tmp = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (tmp == NULL) {
        free(primes_u64);
        free(dp);
        return;
    }
    size_t tmp_len = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes_u64[i];
        if (!dp[(size_t)p]) {
            tmp[tmp_len++] = p;
        }
    }

    /* 5. Store the resulting list. */
    unsummable_primes = tmp;
    unsummable_count  = tmp_len;

    /* 6. Clean up temporary data. */
    free(primes_u64);
    free(dp);
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    init_sequence();                       /* initialise once */

    if (n < 1 || n > unsummable_count)    /* out of range */
        return (seq_t)0;

    return (seq_t)unsummable_primes[n - 1];
}
