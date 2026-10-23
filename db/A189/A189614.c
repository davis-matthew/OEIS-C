/*
 * A189614  Number of nX6 binary arrays without the pattern 0 1 0 diagonally,
 *          antidiagonally or horizontally.
 * https://oeis.org/A189614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×6 binary arrays avoiding the pattern 0 1 0 horizontally,
   diagonally, or anti‑diagonally. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COLS 6
#define MAX_PATTERNS (1 << MAX_COLS)

static bool is_allowed_row(uint8_t pat) {
    /* no horizontal 0 1 0 */
    for (int i = 0; i <= MAX_COLS - 3; ++i) {
        if (((pat >> i) & 7) == 0b010u) return false;
    }
    return true;
}

static bool allowed_triple(uint8_t p, uint8_t q, uint8_t r) {
    /* diagonal down‑right and anti‑diagonal down‑left */
    for (int i = 0; i <= MAX_COLS - 3; ++i) {
        /* down‑right */
        if ((((p >> i) & 1u) == 0u) &&
            (((q >> (i + 1)) & 1u) == 1u) &&
            (((r >> (i + 2)) & 1u) == 0u))
            return false;
        /* down‑left */
        if ((((p >> (i + 2)) & 1u) == 0u) &&
            (((q >> (i + 1)) & 1u) == 1u) &&
            (((r >> i) & 1u) == 0u))
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Build list of allowed row patterns */
    uint8_t allowed[MAX_PATTERNS];
    int A = 0;
    for (int pat = 0; pat < MAX_PATTERNS; ++pat) {
        if (is_allowed_row((uint8_t)pat)) {
            allowed[A++] = (uint8_t)pat;
        }
    }

    /* Map pattern to its compact index */
    int patIdx[MAX_PATTERNS];
    for (int i = 0; i < MAX_PATTERNS; ++i) patIdx[i] = -1;
    for (int i = 0; i < A; ++i) patIdx[allowed[i]] = i;

    /* Direct results for small n */
    if (n == 1) return (seq_t)A;
    if (n == 2) return (seq_t)A * (seq_t)A;

    /* Transition masks: for each pair (i,j) store bitmask of allowed k */
    uint64_t trans[MAX_PATTERNS][MAX_PATTERNS];
    for (int i = 0; i < A; ++i) {
        for (int j = 0; j < A; ++j) {
            uint64_t mask = 0;
            for (int k = 0; k < A; ++k) {
                if (allowed_triple(allowed[i], allowed[j], allowed[k]))
                    mask |= (uint64_t)1u << k;
            }
            trans[i][j] = mask;
        }
    }

    /* DP tables */
    seq_t dpPrev[MAX_PATTERNS][MAX_PATTERNS];
    seq_t dpNext[MAX_PATTERNS][MAX_PATTERNS];

    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            dpPrev[i][j] = (seq_t)0;

    /* Initialize for n = 2 */
    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            dpPrev[i][j] = (seq_t)1;

    /* Iterate through remaining rows */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < A; ++i)
            for (int j = 0; j < A; ++j)
                dpNext[i][j] = (seq_t)0;

        for (int i = 0; i < A; ++i) {
            for (int j = 0; j < A; ++j) {
                seq_t cnt = dpPrev[i][j];
                if (!cnt) continue;
                uint64_t mask = trans[i][j];
                while (mask) {
                    uint64_t lsb = mask & -mask;
                    int k = __builtin_ctzll(mask);
                    dpNext[j][k] += cnt;
                    mask ^= lsb;
                }
            }
        }

        /* swap dpPrev and dpNext */
        for (int i = 0; i < A; ++i)
            for (int j = 0; j < A; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* Sum all final states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            total += dpPrev[i][j];

    return total;
}
