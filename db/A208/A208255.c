/*
 * A208255  Number of nX5 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the linear recurrence
 *   a(n) = 171*a(n-1) – 3370*a(n-2) + 29805*a(n-3) – 149108*a(n-4)
 *          + 447784*a(n-5) – 778496*a(n-6) + 649216*a(n-7) – 114688*a(n-8)
 * for n > 9.
 *
 * Initial values:
 *   a(1) = 41
 *   a(2) = 6144
 *   a(3) = 920928
 *   a(4) = 137947632
 *   a(5) = 20662697664
 *   a(6) = 3094987890864
 *   a(7) = 463586569433280
 *   a(8) = 69438884409258336
 *   a(9) = 10400988693344761104
 *
 * The function uses memoisation so that calls can be made in any order.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Prefix: seq_t and idx_t are defined by the build system. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Initial 9 terms, stored as static constants. */
    static const seq_t base[9] = {
        (seq_t)41ULL,
        (seq_t)6144ULL,
        (seq_t)920928ULL,
        (seq_t)137947632ULL,
        (seq_t)20662697664ULL,
        (seq_t)3094987890864ULL,
        (seq_t)463586569433280ULL,
        (seq_t)69438884409258336ULL,
        (seq_t)10400988693344761104ULL   /* a(9) */
    };

    /* Coefficients are split into positive and negative groups to avoid signed
     * arithmetic on an unsigned `seq_t`.  Each multiplication fits in the
     * target type when the values themselves fit. */
    static const seq_t coeff_pos[4] = {
        (seq_t)171ULL,
        (seq_t)29805ULL,
        (seq_t)447784ULL,
        (seq_t)649216ULL
    };
    static const seq_t coeff_neg[4] = {
        (seq_t)3370ULL,
        (seq_t)149108ULL,
        (seq_t)778496ULL,
        (seq_t)114688ULL
    };

    /* Dynamically allocated cache for memoisation.  The cache holds values
     * for indices 1..cache_len (1‑based).  cache[i-1] == a(i). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* First initialisation: copy the first nine terms. */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = base[i];
    }

    /* If the requested term is already in the cache, return it immediately. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Expand the cache to accommodate the requested term. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (size_t idx = cache_len; idx < new_len; ++idx) {
        /* idx is 0‑based: term number is idx+1.  The recurrence needs
         * the previous eight terms (which are already in the cache). */
        seq_t sum_pos = coeff_pos[0] * cache[idx - 1];
        sum_pos += coeff_pos[1] * cache[idx - 3];
        sum_pos += coeff_pos[2] * cache[idx - 5];
        sum_pos += coeff_pos[3] * cache[idx - 7];

        seq_t sum_neg = coeff_neg[0] * cache[idx - 2];
        sum_neg += coeff_neg[1] * cache[idx - 4];
        sum_neg += coeff_neg[2] * cache[idx - 6];
        sum_neg += coeff_neg[3] * cache[idx - 8];

        cache[idx] = sum_pos - sum_neg;   /* guaranteed non‑negative */
    }

    cache_len = new_len;
    return cache[n - 1];
}
