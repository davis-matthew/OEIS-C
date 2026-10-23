/*
 * A115170  Non-cubefree numbers k such that 2k+1 is also non-cubefree
 *          (A046099).
 * https://oeis.org/A115170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115170: numbers k such that k and 2k+1 are non‑cubefree.
   The function builds the sequence on demand and caches the results. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The function a(n) returns the nth element of the sequence, as
   defined by OEIS, with a(1) = 40. */

/* static cache for the sequence values */
static seq_t *term_cache = NULL;
static idx_t cache_size = 0;

/* --------------------------------------------------------------------- */
/*  Helper: extend the cache to contain at least 'need' terms.           */
/*  The method sieves up to an upper bound that grows until enough      */
/*  terms are found.                                                     */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t need)
{
    if (cache_size >= need)
        return;                     /* already enough */

    /* Old cache is discarded when a larger one is found */
    while (true) {
        /* Rough upper estimate for the k value examined. */
        idx_t limit   = need * 200ULL + 2000ULL;
        idx_t up_to   = 2 * limit + 1;          /* for 2k+1 */

        /* Mark cubefree status. */
        uint8_t *cubefree = (uint8_t *)calloc((size_t)(up_to + 1), 1);
        if (!cubefree)
            return;
        for (idx_t i = 0; i <= up_to; ++i)
            cubefree[i] = 1;

        /* Find largest prime whose cube fits in up_to. */
        uint32_t max_prime = 2;
        while ((seq_t)(max_prime) * max_prime * max_prime <= up_to)
            ++max_prime;
        if (max_prime > 2) --max_prime;

        /* Simple prime sieve up to max_prime. */
        bool *is_prime = (bool *)calloc((size_t)(max_prime + 1), 1);
        if (!is_prime) {
            free(cubefree);
            return;
        }
        for (uint32_t i = 2; i <= max_prime; ++i)
            is_prime[i] = true;
        for (uint32_t i = 2; i * i <= max_prime; ++i)
            if (is_prime[i])
                for (uint32_t j = i * i; j <= max_prime; j += i)
                    is_prime[j] = false;

        /* Mark numbers that are not cubefree. */
        for (uint32_t p = 2; p <= max_prime; ++p) {
            if (!is_prime[p])
                continue;
            seq_t cube = (seq_t)p * p * p;
            for (idx_t j = (idx_t)cube; j <= up_to; j += (idx_t)cube)
                cubefree[j] = 0;
        }
        free(is_prime);

        /* Collect terms. */
        seq_t *new_cache = (seq_t *)calloc((size_t)need, sizeof(seq_t));
        if (!new_cache) {
            free(cubefree);
            return;
        }
        idx_t count = 0;
        for (idx_t k = 1; k <= limit && count < need; ++k) {
            if (!cubefree[k] && !cubefree[2 * k + 1]) {
                new_cache[count++] = (seq_t)k;
            }
        }
        free(cubefree);

        /* If we found enough terms, replace the cache. */
        if (count >= need) {
            free(term_cache);
            term_cache = new_cache;
            cache_size = count;
            return;
        }

        /* Not enough terms – free the temporary cache and try again. */
        free(new_cache);
        need *= 2;   /* increase requirement and loop again */
    }
}

/* --------------------------------------------------------------------- */
/*  Public interface required by the build system.                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* the sequence is defined for n≥1 */

    ensure_terms(n);
    if (n <= cache_size)
        return term_cache[n - 1];
    /* In the unlikely event of an error, return 0. */
    return (seq_t)0;
}
