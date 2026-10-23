/*
 * A008036  Coordination sequence T1 for Zeolite Code AST.
 * https://oeis.org/A008036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 4*a(n-2) + 7*a(n-3) - 8*a(n-4) + 10*a(n-5) * -
 *            10*a(n-6) + 8*a(n-7) - 7*a(n-8) + 4*a(n-9) - 2*a(n-10) * +
 *            a(n-11) (for n ≥ 12). a(0)…a(11) are given in the OEIS entry
 * Range:     u8 n <= 11, u16 n <= 183, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/mul_small.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)19, (seq_t)34,
        (seq_t)48, (seq_t)66, (seq_t)96, (seq_t)127, (seq_t)151,
        (seq_t)183, (seq_t)232
    };

    /* Cache of already computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Get terms for indices smaller than the known block. */
    if (n < 12) return init[n];

    /* Initialise cache if first call. */
    if (cache == NULL) {
        cap = 12;
        cache = seq_realloc(NULL, 0, cap);
        memcpy(cache, init, 12 * sizeof(seq_t));
    }

    /* Grow cache to hold up to index n. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;   /* double until big enough */
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (size_t i = 12; i <= (size_t)n; ++i) {
        /* Positive contribution terms */
        seq_t sum_pos = mul_small(cache[i-1], 2);
        sum_pos += mul_small(cache[i-3], 7);
        sum_pos += mul_small(cache[i-5], 10);
        sum_pos += mul_small(cache[i-7], 8);
        sum_pos += mul_small(cache[i-9], 4);
        /* a(n-11) has coefficient +1 */
        sum_pos += cache[i-11];

        /* Negative contribution terms */
        seq_t sum_neg = mul_small(cache[i-2], 4);
        sum_neg += mul_small(cache[i-4], 8);
        sum_neg += mul_small(cache[i-6], 10);
        sum_neg += mul_small(cache[i-8], 7);
        sum_neg += mul_small(cache[i-10], 2);

        cache[i] = sum_pos - sum_neg;   /* guaranteed non‑negative */
    }

    return cache[(size_t)n];
}
