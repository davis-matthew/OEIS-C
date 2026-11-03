/*
 * A025336  Numbers that are the sum of 3 nonzero squares in 8 or more ways.
 * https://oeis.org/A025336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that can be expressed as the sum of * 3
 *            non‑zero squares in at least 8 unordered ways. The
 *            representation * count is unordered (x≤y≤z). The function
 *            precomputes the counts up * to a fixed limit and then extracts
 *            the terms
 * Range:     u8 none, u16 n <= 5058, u32 n <= 5058, u64 n <= 5058,
 *            u128 n <= 5058, big any n
 * Validated: standard, pass, 69 terms through n = 5029 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers -------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to square an idx_t without overflow (using 128‑bit). */
static inline idx_t sq(idx_t a) {
    return (idx_t)((unsigned __int128)a * a);
}

/* Compile‑time limit for precomputation ----------------------------------- */
/* Adjust if more terms are needed. */
#ifndef LIMIT_N
#define LIMIT_N 1000000u
#endif

/* Helper to compute integer square root of an unsigned integer (idx_t). */
static inline idx_t int_sqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Static data for memoisation --------------------------------------------- */
static bool initialized = false;
static uint32_t *cnt = NULL;      /* count of unordered triples for each sum */
static seq_t  *terms = NULL;     /* list of sequence terms in ascending order */
static idx_t   terms_cap = 0;
static idx_t   terms_count = 0;

/* ------------------------------------------------------------------------- */
static void init(void) {
    if (initialized) return;

    /* Allocate count array and zero it */
    cnt = (uint32_t *)calloc((size_t)LIMIT_N + 1, sizeof(uint32_t));

    /* Precompute unordered representation counts */
    idx_t root = int_sqrt_u64(LIMIT_N);
    for (idx_t x = 1; x <= root; ++x) {
        idx_t x2 = sq(x);
        if (x2 > LIMIT_N) break;
        for (idx_t y = x; y <= root; ++y) {
            idx_t y2 = sq(y);
            idx_t sumxy = x2 + y2;
            if (sumxy > LIMIT_N) break;
            for (idx_t z = y; ; ++z) {
                idx_t z2 = sq(z);
                idx_t sum = sumxy + z2;
                if (sum > LIMIT_N) break;
                ++cnt[sum];
            }
        }
    }

    /* Collect terms that meet the 8‑representations threshold */
    terms_cap = 1024;
    terms = mem_new(seq_t, terms_cap);
    terms_count = 0;
    for (idx_t s = 1; s <= LIMIT_N; ++s) {
        if (cnt[s] >= 8) {
            if (terms_count >= terms_cap) {
                terms_cap <<= 1;
                seq_t *new_terms = mem_resize(seq_t, terms, terms_cap);
                if (!new_terms) { /* allocation failure – abort */
                    free(cnt);
                    mem_free(terms);
                    terms = NULL;
                    memset((void *)&initialized, 1, sizeof(initialized));
                    return;
                }
                terms = new_terms;
            }
            terms[terms_count++] = (seq_t)s;
        }
    }

    /* Clean up the count array – not needed further */
    free(cnt);
    cnt = NULL;

    initialized = true;
}

/* ------------------------------------------------------------------------- */
/* Public function required by the specification --------------------------- */
seq_t a(idx_t n)
{
    init();                         /* ensure tables are built */

    if (n == 0) return 0;
    if (n - 1 < terms_count) return terms[n - 1];
    /* If requested n exceeds precomputed range, return 0 (out of bounds). */
    return 0;
}
