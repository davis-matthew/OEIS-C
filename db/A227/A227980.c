/*
 * A227980  Numbers n such that sigma(n-1) - sigma(n+1) >= sigma(n); where
 *          sigma(n) = A000203(n) = sum of the divisors of n.
 * https://oeis.org/A227980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1030, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227980: numbers n with sigma(n‑1) – sigma(n+1) >= sigma(n) */
#include <stdlib.h>
#include <stdint.h>

/* sigmas of all numbers up to a fixed limit */
static seq_t *sigma = NULL;
static size_t sigma_size = 0;

/* sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t last_checked = 0;

/* helper to grow the cache */
static void grow_cache(void) {
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* build divisor sums up to LIMIT */
static void build_sigma(size_t LIMIT) {
    sigma_size = LIMIT + 2;                     /* allow indices up to LIMIT+1 */
    sigma = (seq_t *)calloc(sigma_size, sizeof(seq_t));

    for (size_t i = 1; i < sigma_size; ++i) {
        for (size_t j = i; j < sigma_size; j += i) {
            sigma[j] += (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    /* Ensure sigma and cache are prepared */
    if (sigma == NULL) {
        /* Choose a limit that comfortably contains at least the first 10000 terms */
        build_sigma(600000);                    /* about 600 k for n and n±1 */
    }

    /* 1‑based indexing: a(1) is the first element of seq_cache */
    const idx_t target = n;
    if (target == 0) return 0;

    /* Generate sequence elements until we have enough */
    while (seq_len < target) {
        ++last_checked;                         /* next candidate n */

        /* guard against running out of precomputed sigma */
        if ((size_t)last_checked + 1 >= sigma_size) {
            /* not expected for requested n within limits */
            return 0;
        }

        seq_t s_prev = sigma[last_checked - 1];
        seq_t s_cur  = sigma[last_checked];
        seq_t s_next = sigma[last_checked + 1];

        /* avoid unsigned underflow: check s_prev >= s_next + s_cur */
        seq_t sum_next_cur = s_next + s_cur;
        if (s_prev >= sum_next_cur) {
            if (seq_len == seq_cap) grow_cache();
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_cache[target - 1];
}
