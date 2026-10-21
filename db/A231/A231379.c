/*
 * A231379  Number of nX5 0..1 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with no 0 entry having a
 *            strict majority of 1s among its horizontal/vertical neighbors.
 *            Computed by transfer‑matrix dynamic programming over consecutive
 *            rows. Initial values: a(1)=12, a(2)=113
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

/* 5‑cell row mask 0 … 31 */
#define ROWS 32
#define ST  (ROWS * ROWS)

/* Pre‑computation of row validity tables */
static bool first_valid[ROWS][ROWS];
static bool last_valid[ROWS][ROWS];
static bool trans[ROWS][ROWS][ROWS];
static bool tables_built = false;

/* Check whether a zero cell violates the rule */
static bool row_ok(uint32_t prev, uint32_t cur, uint32_t next,
                   bool has_prev, bool has_next)
{
    for (int c = 0; c < 5; ++c) {
        uint32_t v = (cur >> c) & 1U;
        unsigned neigh = 0, ones = 0;

        /* horizontal neighbours */
        if (c > 0)   { neigh++; ones += (cur >> (c - 1)) & 1U; }
        if (c < 4)   { neigh++; ones += (cur >> (c + 1)) & 1U; }

        /* vertical neighbours */
        if (has_prev) { neigh++; ones += (prev >> c) & 1U; }
        if (has_next) { neigh++; ones += (next >> c) & 1U; }

        /* a zero is illegal if ones > neigh/2 (strict majority) */
        if (v == 0 && ones > neigh / 2U)
            return false;
    }
    return true;
}

static void build_tables(void)
{
    for (uint32_t r1 = 0; r1 < ROWS; ++r1)
    for (uint32_t r2 = 0; r2 < ROWS; ++r2) {
        first_valid[r1][r2] = row_ok(0, r1, r2, false, true);
        last_valid [r1][r2] = row_ok(r1, r2, 0, true, false);
    }

    for (uint32_t p = 0; p < ROWS; ++p)
    for (uint32_t c = 0; c < ROWS; ++c)
    for (uint32_t n = 0; n < ROWS; ++n)
        trans[p][c][n] = row_ok(p, c, n, true, true);

    tables_built = true;
}

/* static storage for dynamic programming */
static seq_t *dp  = NULL;
static seq_t *ndp = NULL;
static size_t dp_cap = 0;

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!tables_built)
        build_tables();

    /* Allocate or reallocate DP matrices */
    if (!dp || dp_cap < ST) {
        if (dp) seq_free(dp);
        if (ndp) seq_free(ndp);
        dp  = seq_alloc(ST);
        ndp = seq_alloc(ST);
        dp_cap = ST;
    }

    /* Special case n==1 */
    if (n == 1) {
        seq_t res = 0;
        for (uint32_t r = 0; r < ROWS; ++r)
            if (row_ok(0, r, 0, false, false))
                res += (seq_t)1;
        return res;
    }

    /* Initialise dp for the first two rows */
    for (size_t i = 0; i < ST; ++i) dp[i] = 0;
    for (uint32_t r1 = 0; r1 < ROWS; ++r1)
    for (uint32_t r2 = 0; r2 < ROWS; ++r2)
        if (first_valid[r1][r2])
            dp[r1 * ROWS + r2] = 1;

    /* Process remaining rows */
    for (idx_t row = 3; row <= n; ++row) {
        for (size_t i = 0; i < ST; ++i) ndp[i] = 0;
        for (uint32_t prev = 0; prev < ROWS; ++prev) {
            size_t base = prev * ROWS;
            for (uint32_t cur = 0; cur < ROWS; ++cur) {
                seq_t val = dp[base + cur];
                if (val == 0) continue;          /* skip zero counts */
                for (uint32_t next = 0; next < ROWS; ++next)
                    if (trans[prev][cur][next])
                        ndp[cur * ROWS + next] += val;
            }
        }
        /* swap dp and ndp */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* Final row validation (row n) */
    seq_t result = 0;
    for (uint32_t prev = 0; prev < ROWS; ++prev) {
        size_t base = prev * ROWS;
        for (uint32_t cur = 0; cur < ROWS; ++cur)
            if (last_valid[prev][cur])
                result += dp[base + cur];
    }
    return result;
}
