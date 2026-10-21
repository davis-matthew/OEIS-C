/*
 * A206245  Number of partitions of n into repunit powers, cf. A083278.
 * https://oeis.org/A206245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into powers of repunits A
 *            repunit is 1, 11, 111, ...; a repunit power is R_k^e for some e
 *            ≥ 1
 * Range:     u8 n <= 494, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Auxiliary comparator for qsort over idx_t */
static int cmp_idx_t(const void *a, const void *b) {
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Memoised DP table */
static seq_t *dp = NULL;
static size_t dp_len = 0;

/* Return the number of partitions of n into repunit powers. */
seq_t a(idx_t n) {
    /* If already computed up to n, return the cached value. */
    if (n < dp_len) {
        return dp[n];
    }

    idx_t max_n = n;

    /* Generate all repunit powers ≤ max_n, stored as idx_t (no overflow). */
    size_t parts_cap = 0, parts_len = 0;
    idx_t *parts = NULL;

    /* Helper macro for growing the parts array. */
    #define ENSURE_PARTS_CAP(n)                               \
        if ((parts_len) >= (parts_cap)) {                     \
            (parts_cap) = (parts_cap) ? (parts_cap) << 1 : 1; \
            parts = (idx_t *)realloc(parts, (parts_cap) * sizeof(idx_t)); \
        }

    /* Base repunit 1. */
    if (max_n >= 1) {
        ENSURE_PARTS_CAP(1);
        parts[parts_len++] = 1;
    }

    /* Generate repunits R_k = 111...1 (k digits) for k ≥ 2. */
    idx_t rep = 0;
    for (int k = 1; ; ++k) {
        rep = rep * 10 + 1;
        if (rep > max_n) {
            break;
        }
        idx_t base = rep;
        if (base == 1) {            /* already added once */
            continue;
        }

        /* Add the base itself. */
        ENSURE_PARTS_CAP(1);
        parts[parts_len++] = base;

        /* Add higher powers base^e as long as they fit. */
        idx_t cur = base;
        while (cur <= max_n / base) {
            cur *= base;          /* product fits into idx_t */
            ENSURE_PARTS_CAP(1);
            parts[parts_len++] = cur;
        }
    }

    /* Sort and deduplicate the parts list. */
    if (parts_len > 1) {
        qsort(parts, parts_len, sizeof(idx_t), cmp_idx_t);
        size_t uniq = 1;            /* parts[0] is already unique */
        for (size_t i = 1; i < parts_len; ++i) {
            if (parts[i] != parts[i - 1]) {
                parts[uniq++] = parts[i];
            }
        }
        parts_len = uniq;
    }

    /* Allocate new DP array up to max_n. */
    seq_t *new_dp = (seq_t *)malloc(sizeof(seq_t) * (max_n + 1));
    for (idx_t i = 0; i <= max_n; ++i) {
        new_dp[i] = (seq_t)0;
    }
    new_dp[0] = (seq_t)1;   /* empty partition */

    /* Classic coin‑change DP (unordered partitions). */
    for (size_t pi = 0; pi < parts_len; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= max_n; ++i) {
            new_dp[i] += new_dp[i - p];
        }
    }

    /* Replace old cache with the newly computed one. */
    free(dp);
    dp = new_dp;
    dp_len = max_n + 1;

    /* Clean up the parts array. */
    free(parts);

    return dp[n];
}
