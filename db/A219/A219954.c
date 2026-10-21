/*
 * A219954  (A160414(n)-1)/4, n >= 1.
 * https://oeis.org/A219954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 0 * a(n) = sum_{k=2..n} (3^{popcnt(k-1)} - (k/2 if k is
 *            power of 2)), * where popcnt(m) is the binary population count
 *            of m. * This implements the exact definition of OEIS A219954
 * Range:     u8 n <= 18, u16 n <= 267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_power_of_two_u64.h"

/* Basic popcount for 64‑bit unsigned integers. */
static inline unsigned popcnt_u64(uint64_t x) {
    return __builtin_popcountll(x);
}

/* Cache for the sequence values.  cache[i] = a(i).  */
static seq_t *cache = NULL;      /* NULL until first allocation   */
static idx_t  max_computed = 0;  /* largest n for which a(n) is cached */

/* Precomputed powers of 3 up to 3^64 (enough for any idx_t < 2^64). */
static int pow3_initialized = 0;
static seq_t pow3[65];

static void init_pow3(void) {
    if (pow3_initialized) return;
    pow3[0] = (seq_t)1;
    for (int i = 1; i < 65; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow3_initialized = 1;
}

/* Ensure that cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n <= max_computed) return;

    /* Grow or create the cache array.  We keep index 0 unused. */
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE); /* safe‑guard */
    } else {
        new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* If this is the first allocation, set cache[0] = 0.  */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        if (n >= 1) cache[1] = (seq_t)0;     /* a(1) = 0 */
    }

    /* Compute new terms, starting at the first missing index. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i >= 2 only, the term is defined as below. */
        seq_t term;
        if (i >= 2) {
            unsigned pc = popcnt_u64(i - 1);           /* popcnt(k-1) */
            term = pow3[pc];
            if (is_power_of_two_u64(i)) {
                term -= (seq_t)(i / 2);                /* subtract k/2 */
            }
            cache[i] = cache[i - 1] + term;
        } else { /* i == 1, already set to 0 */
            cache[i] = (seq_t)0;
        }
    }

    max_computed = n;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* not used by OEIS, safe guard */
    init_pow3();
    ensure_cache(n);
    return cache[n];
}
