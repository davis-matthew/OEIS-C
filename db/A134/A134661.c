/*
 * A134661  Number of odd coefficients in (1 + x + x^3)^n.
 * https://oeis.org/A134661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd coefficients in (1 + x + x^3)^n
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* External integer types */
extern seq_t a(idx_t n);

/* Helper to count bits in a uint64_t */
static inline seq_t popcount_u64(uint64_t v) {
    return (seq_t)__builtin_popcountll(v);
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Serializable static state */
    static seq_t *counts = NULL;          /* a[0..max_n] */
    static size_t counts_cap = 0;
    static idx_t counts_len = 0;          /* number of entries available */

    static uint64_t *poly_cur = NULL;     /* current polynomial bitset */
    static size_t poly_words = 0;         /* number of 64‑bit words of poly_cur */
    static idx_t cur_n = 0;               /* index of poly_cur */

    /* Initialisation */
    if (!poly_cur) {
        poly_words = 1;
        poly_cur = (uint64_t*)calloc(poly_words, sizeof(uint64_t));
        if (!poly_cur) return 0;          /* allocation failure */
        poly_cur[0] = 1ULL;               /* (1)^0 = 1  */
        cur_n = 0;

        counts_cap = 1;
        counts = (seq_t*)malloc(counts_cap * sizeof(seq_t));
        if (!counts) return 0;
        counts[0] = 1;                    /* a(0) = 1 */
        counts_len = 1;
    }

    /* Return cached value if already computed */
    if (n < counts_len)
        return counts[n];

    /* Grow counts array if necessary */
    if (n >= counts_cap) {
        size_t new_cap = (counts_cap + 1) * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(counts, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        counts = tmp;
        counts_cap = new_cap;
    }

    /* Compute from cur_n+1 up to n */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        /* Degrees */
        idx_t deg_new = (idx_t)3 * i;                 /* maximum exponent = 3*i */
        size_t words_new = (size_t)((deg_new + 1 + 63) / 64); /* ceil((deg_new+1)/64) */

        /* Allocate next polynomial buffer */
        uint64_t *poly_next = (uint64_t*)calloc(words_new, sizeof(uint64_t));
        if (!poly_next) return 0;

        /* Shift 0 and XOR */
        for (size_t j = 0; j < poly_words; ++j)
            poly_next[j] ^= poly_cur[j];

        /* Shift 1 */
        for (size_t j = 0; j < poly_words; ++j) {
            uint64_t v = poly_cur[j];
            poly_next[j] ^= v << 1;
            if (j + 1 < words_new)
                poly_next[j + 1] ^= v >> 63;
        }

        /* Shift 3 */
        for (size_t j = 0; j < poly_words; ++j) {
            uint64_t v = poly_cur[j];
            poly_next[j] ^= v << 3;
            if (j + 1 < words_new)
                poly_next[j + 1] ^= v >> 61;
        }

        /* Count odd coefficients (bits set to 1) */
        seq_t cnt = 0;
        for (size_t j = 0; j < words_new; ++j)
            cnt += popcount_u64(poly_next[j]);

        /* Store result */
        counts[i] = cnt;
        counts_len = (idx_t)(i + 1);

        /* Prepare for next iteration */
        free(poly_cur);
        poly_cur = poly_next;
        poly_words = words_new;
        cur_n = i;
    }

    return counts[n];
}
