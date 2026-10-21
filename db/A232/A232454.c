/*
 * A232454  Number of nX3 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, vertically, diagonally or
 *          antidiagonally.
 * https://oeis.org/A232454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} such that every 0 has a
 *            neighboring 1 and every 1 has a neighboring 2 (neighbors are
 *            horizontal, vertical or diagonal). Computed by dynamic
 *            programming over rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   need_neighbour
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/need_neighbour.h"

/* 3‑letter rows: 0..2 in each of the 3 columns. */
#define PAT_COUNT 27
#define MASK_MAX 8   /* 3‑bit mask for unsatisfied columns */

/* All 27 possible row patterns. */
static const uint8_t patterns[PAT_COUNT][3] = {
    {0,0,0},{0,0,1},{0,0,2},{0,1,0},{0,1,1},{0,1,2},{0,2,0},{0,2,1},{0,2,2},
    {1,0,0},{1,0,1},{1,0,2},{1,1,0},{1,1,1},{1,1,2},{1,2,0},{1,2,1},{1,2,2},
    {2,0,0},{2,0,1},{2,0,2},{2,1,0},{2,1,1},{2,1,2},{2,2,0},{2,2,1},{2,2,2}
};

/* Pre‑computed tables. */
static uint8_t  first_mask[PAT_COUNT];
static uint8_t  trans_mask[PAT_COUNT][PAT_COUNT];
static bool     comp_mask[PAT_COUNT][PAT_COUNT][MASK_MAX];

/* Compute unsatisfied mask for the first row: only horizontal neighbours. */
static uint8_t compute_first_mask(uint8_t pat) {
    uint8_t mask = 0;
    for (int j = 0; j < 3; ++j) {
        uint8_t val = patterns[pat][j];
        if (val == 2) continue;                /* 2 needs nothing */
        uint8_t need = (val == 0 ? 1 : 2);      /* 0 needs 1, 1 needs 2 */
        bool sat = false;
        if (j > 0 && need_neighbour(patterns[pat][j-1], need)) sat = true;
        if (j < 2 && need_neighbour(patterns[pat][j+1], need)) sat = true;
        if (!sat) mask |= (1u << j);
    }
    return mask;
}

/* Compute unsatisfied mask for 'cur' given previous row 'prev'. */
static uint8_t compute_transition_mask(uint8_t cur, uint8_t prev) {
    uint8_t mask = 0;
    for (int j = 0; j < 3; ++j) {
        uint8_t val = patterns[cur][j];
        if (val == 2) continue;
        uint8_t need = (val == 0 ? 1 : 2);
        bool sat = false;
        /* horizontal within current row */
        if (j > 0 && need_neighbour(patterns[cur][j-1], need)) sat = true;
        if (j < 2 && need_neighbour(patterns[cur][j+1], need)) sat = true;
        /* neighbors from previous row (vertical/diagonal) */
        if (!sat) {
            if (j > 0 && need_neighbour(patterns[prev][j-1], need)) sat = true;
            if (need_neighbour(patterns[prev][j],   need))           sat = true;
            if (j < 2 && need_neighbour(patterns[prev][j+1], need)) sat = true;
        }
        if (!sat) mask |= (1u << j);
    }
    return mask;
}

/* Check if a cur row satisfies all unsatisfied cells of prev row. */
static bool compute_compatibility(uint8_t prev, uint8_t cur, uint8_t prevMask) {
    for (int j = 0; j < 3; ++j) {
        if (!(prevMask & (1u << j))) continue;   /* already satisfied */
        uint8_t val = patterns[prev][j];
        uint8_t need = (val == 0 ? 1 : 2);
        bool sat = false;
        /* neighbors in current row (vertical/diagonal) */
        if (j > 0 && need_neighbour(patterns[cur][j-1], need)) sat = true;
        if (need_neighbour(patterns[cur][j],   need))           sat = true;
        if (j < 2 && need_neighbour(patterns[cur][j+1], need)) sat = true;
        if (!sat) return false;
    }
    return true;
}

/* One‑time table initialisation. */
static bool tables_done = false;
static void init_tables(void) {
    if (tables_done) return;
    for (uint8_t p = 0; p < PAT_COUNT; ++p) {
        first_mask[p] = compute_first_mask(p);
        for (uint8_t q = 0; q < PAT_COUNT; ++q) {
            trans_mask[p][q] = compute_transition_mask(p, q);
            for (uint8_t m = 0; m < MASK_MAX; ++m)
                comp_mask[p][q][m] = compute_compatibility(p, q, m);
        }
    }
    tables_done = true;
}

/* Public entry point. */
seq_t a(idx_t n) {
    init_tables();

    /* dp[pat][mask] : number of ways up to current row. */
    seq_t dp[PAT_COUNT][MASK_MAX]   = {{0}};
    seq_t next_dp[PAT_COUNT][MASK_MAX] = {{0}};

    /* First row */
    for (uint8_t p = 0; p < PAT_COUNT; ++p) {
        uint8_t m = first_mask[p];
        dp[p][m] = 1;
    }

    /* Remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next_dp */
        for (uint8_t p = 0; p < PAT_COUNT; ++p)
            for (uint8_t m = 0; m < MASK_MAX; ++m)
                next_dp[p][m] = 0;

        for (uint8_t prev = 0; prev < PAT_COUNT; ++prev) {
            for (uint8_t prevMask = 0; prevMask < MASK_MAX; ++prevMask) {
                seq_t ways = dp[prev][prevMask];
                if (!ways) continue;
                for (uint8_t cur = 0; cur < PAT_COUNT; ++cur) {
                    if (!comp_mask[prev][cur][prevMask]) continue;
                    uint8_t newMask = trans_mask[cur][prev];
                    next_dp[cur][newMask] += ways;
                }
            }
        }

        /* copy next_dp -> dp */
        for (uint8_t p = 0; p < PAT_COUNT; ++p)
            for (uint8_t m = 0; m < MASK_MAX; ++m)
                dp[p][m] = next_dp[p][m];
    }

    /* Sum all completions with mask zero (all satisfied). */
    seq_t total = 0;
    for (uint8_t p = 0; p < PAT_COUNT; ++p)
        total += dp[p][0];

    return total;
}
