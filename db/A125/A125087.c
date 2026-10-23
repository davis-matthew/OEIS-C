/*
 * A125087  a(1)=1. For n>1, a(n) = the number of terms (from among the first
 *          n-1 terms of the sequence) that are equal to at least one exponent
 *          in the prime factorization of n.
 * https://oeis.org/A125087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = sum of counts of previous a-values equal
 *            to each * distinct exponent appearing in the prime factorization
 *            of n. * (Each exponent is counted only once even if it occurs
 *            for multiple prime bases.)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* freq[e] = number of previous a(i) equal to e (we only need to keep
     * counts for e ≤ 64, the maximum exponent that can appear for the
     * n ranges we support). */
    static size_t freq[65] = {0};

    /* Safety for zero index – not used by the sequence */
    if (n == 0) return 0;

    /* Initialise with a(1)=1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = 1;            /* a(1) = 1 */
        freq[1] = 1;             /* record the occurrence of value 1 */
        cache_size = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size) {
        return cache[n - 1];
    }

    /* Extend cache up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;

    for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
        /* Factorise idx */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64((uint64_t)idx, primes, exps, &cnt);

        /* Sum frequencies for distinct exponents */
        bool seen[65] = {false};
        size_t temp_val = 0;
        for (size_t j = 0; j < cnt; ++j) {
            size_t exp = (size_t)exps[j];
            if (exp <= 64 && !seen[exp]) {
                temp_val += freq[exp];
                seen[exp] = true;
            }
        }
        seq_t val = (seq_t)temp_val;
        cache[idx - 1] = val;

        /* Record this value for future distinct‑exponent counting */
        if (val <= 64) {
            freq[(size_t)val] += 1;
        }
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
