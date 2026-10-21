/*
 * A078382  Sum of divisors of numbers which can be written as sum of a
 *          positive square and a positive cube.
 * https://oeis.org/A078382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_of_divisors( A055394(n) ) where A055394 is the
 *            sequence of distinct positive integers of the form a² + b³ (a ≥
 *            1 , b ≥ 1). The values are generated lazily and cached for
 *            future calls. The implementation uses a simple sieve over a
 *            bounded value in order to collect all sums up to the poi...
 * Range:     u8 n <= 30, u16 n <= 2517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

/* Forward declaration for sigma calculation. */
static seq_t sigma_of_uint64(uint64_t n);

/* Cache for the sequence values. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;

/* Expand the cache until it contains at least 'required' terms. */
static void build_sequence(size_t required)
{
    /*  Keep expanding the limit until enough distinct sums are found.  */
    uint64_t limit = 65536;          /* initial bound, can grow */
    const size_t safety = 3000;      /* extra terms to protect against duplicates */

    /* If already built to requested size, nothing to do. */
    if (cache_len >= required)
        return;

    /* Free previous cache if any. */
    if (cache) { free(cache); cache = NULL; cache_len = 0; }

    while (1) {
        /* Mark unique sums with a byte per possible value within the limit. */
        uint8_t *seen = (uint8_t*)calloc(limit + 1, 1);
        if (!seen) abort();          /* memory failure */

        size_t count = 0;
        for (uint64_t a = 1; ; ++a) {
            uint64_t a2 = a * a;
            if (a2 > limit) break;
            for (uint64_t b = 1; ; ++b) {
                uint64_t cube = b * b * b;
                if (cube > limit) break;
                uint64_t sum = a2 + cube;
                if (sum > limit) break;
                if (!seen[sum]) {
                    seen[sum] = 1;
                    ++count;
                }
            }
        }

        /* Are we satisfied?  If so, use this limit to build the cache. */
        if (count >= required + safety) {
            seq_t *new_cache = (seq_t*)malloc(count * sizeof(seq_t));
            if (!new_cache) abort();    /* memory failure */

            size_t idx = 0;
            for (uint64_t v = 1; v <= limit; ++v) {
                if (seen[v]) {
                    new_cache[idx++] = sigma_of_uint64(v);
                }
            }
            cache = new_cache;
            cache_len = idx;

            free(seen);
            break;
        }

        /* Too few terms; double the limit and try again. */
        free(seen);
        limit <<= 1;
    }
}

/* Calculate σ(n) for a 64‑bit integer. */
static seq_t sigma_of_uint64(uint64_t n)
{
    if (n == 0) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        unsigned e = (unsigned)exps[i];
        seq_t term = 1;
        seq_t power = 1;
        for (unsigned j = 0; j < e; ++j) {
            power *= p;
            term += power;
        }
        result *= term;
    }
    return result;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n + 1000 terms for future calls. */
    size_t req = (size_t)n + 1000;
    if (!cache || cache_len < req)
        build_sequence(req);

    /* n is 1‑based – cache is 0‑based. */
    return cache[(size_t)n - 1];
}
