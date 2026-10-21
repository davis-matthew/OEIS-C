/*
 * A094197  First integral ladder to be largest perpendicular-corner-bending
 *          for exactly n distinct pairs of integral corridor widths.
 * https://oeis.org/A094197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   seq_cmp, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094197:  a(n) = d^3 where d is the smallest integer such that
 * 2*n+1 = Π_i (2*e_i+1) and d = Π_i p_i^{e_i} with p_i ≡ 1 (mod 4).
 * The algorithm enumerates all factorizations of (2*n+1) into odd factors
 * ≥ 3, turns them into exponent tuples, computes the associated d and
 * keeps the minimum.  Results are cached. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

/* Small primes ≡ 1 (mod 4); plenty for all factorizations up to n≈10 000. */
static const uint32_t prime1mod4[] = {
    5, 13, 17, 29, 37, 41, 53, 61, 73, 89,
    97, 101, 109, 113, 137, 149, 157, 173, 181, 193,
    197, 233, 241, 257, 269, 277, 281, 293, 313, 337
};
static const size_t prime1mod4_count = sizeof(prime1mod4) / sizeof(prime1mod4[0]);

/* Enumerate all factorizations of the odd integer `rem` into odd factors ≥3.
 * When a complete factorization has been found, compute the corresponding
 * d and update the best candidate. */
static void backtrack(idx_t rem, idx_t start, idx_t *factors, size_t len,
                      seq_t *best_d, bool *found)
{
    if (rem == 1) {
        /* Convert each factor f_i to exponent e_i = (f_i-1)/2. */
        idx_t exps[256];
        for (size_t i = 0; i < len; ++i)
            exps[i] = (factors[i] - 1) / 2;

        /* Sort exponents descending so that larger exponents use
         * the smallest primes. */
        for (size_t i = 1; i < len; ++i) {
            idx_t key = exps[i];
            size_t j = i;
            while (j > 0 && exps[j - 1] < key) {
                exps[j] = exps[j - 1];
                --j;
            }
            exps[j] = key;
        }

        /* Compute d = Π primes[i]^{exps[i]}. */
        seq_t d = 1;
        for (size_t i = 0; i < len; ++i) {
            d = seq_mul(d, seq_pow((seq_t)prime1mod4[i], exps[i]));
        }

        if (!*found || seq_cmp(&d, &(*best_d)) < 0) {
            *best_d = d;
            *found   = true;
        }
        return;
    }

    for (idx_t f = start; f <= rem; f += 2) {     /* odd factors only */
        if (rem % f == 0) {
            factors[len] = f;
            backtrack(rem / f, f, factors, len + 1, best_d, found);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1, harmless */

    /* Memoisation cache.  cache[0] is unused. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;

    if (cache_len <= n) {
        size_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;          /* 0 means “not yet computed” */
        cache_len = new_len;
    }
    if (cache[n] != 0)
        return cache[n];

    /* Find the minimal d for this n.  The odd integer is 2*n+1. */
    idx_t odd = 2 * n + 1;
    idx_t factors[256];
    seq_t best_d = 0;
    bool found   = false;

    backtrack(odd, 3, factors, 0, &best_d, &found);

    /* a(n) = d^3. */
    seq_t result = seq_pow(best_d, 3);
    cache[n] = result;
    return result;
}
