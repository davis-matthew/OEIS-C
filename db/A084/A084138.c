/*
 * A084138  a(n) is the number of times n is in sequence A060715; i.e., there
 *          are exactly a(n) cases where there are exactly n primes between m
 *          and 2m, exclusively, for m > 0.
 * https://oeis.org/A084138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m > 0 such that there are exactly n primes in
 *            (m,2m)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* static cache pointers */
static uint64_t *primes = NULL;                 /* list of primes, not used after init */
static size_t primes_cnt = 0;
static idx_t *pi = NULL;                       /* prime counting function */
static seq_t *hist = NULL;                     /* histogram of prime counts */
static size_t hist_size = 0;                   /* current allocated hist size */
static bool inited = false;

/* pre‑compute all data needed for the sequence */
static void init_cache(void)
{
    if (inited) return;
    inited = true;

    /* Upper bound for m to be considered. Chosen so that all n up to
       ~10000 are covered. */ 
    const idx_t M = 1000000;                     /* m ranges from 1 to M */
    const idx_t limit = 2 * M + 1;               /* must include 2*M */

    /* 1. Build prime list up to limit */
    build_primes_upto(limit, &primes, &primes_cnt);

    /* 2. Allocate and fill isprime array */
    uint8_t *isprime = (uint8_t*)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!isprime) return;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (primes[i] <= limit) isprime[(size_t)primes[i]] = 1;
    }

    /* 3. Build prime counting array pi */
    pi = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!pi) {
        free(isprime);
        return;
    }
    pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi[i] = pi[i-1] + (isprime[(size_t)i] ? 1 : 0);

    /* 4. Build histogram of counts of primes between m and 2m */
    hist_size = 32768;                          /* initial histogram size */
    hist = (seq_t*)calloc(hist_size, sizeof(seq_t));
    if (!hist) {
        free(isprime);
        free(pi);
        return;
    }

    for (idx_t m = 1; m <= M; ++m) {
        idx_t upper = 2 * m - 1;                /* exclusive upper bound 2m */
        idx_t cnt = pi[upper] - pi[m];          /* primes in (m,2m) */

        /* enlarge histogram if needed */
        if ((size_t)cnt >= hist_size) {
            size_t new_size = hist_size;
            while ((size_t)cnt >= new_size) new_size <<= 1;
            seq_t *tmp = (seq_t*)realloc(hist, new_size * sizeof(seq_t));
            if (!tmp) break;                    /* out of memory, stop early */
            /* clear the newly allocated part */
            memset(tmp + hist_size, 0, (new_size - hist_size) * sizeof(seq_t));
            hist = tmp;
            hist_size = new_size;
        }
        ++hist[(size_t)cnt];
    }

    /* 5. Clean up auxiliary data */
    free(isprime);
    free(pi);
    free(primes);
    primes = NULL;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (!inited) init_cache();

    if (n < hist_size) return hist[(size_t)n];
    return (seq_t)0;
}
