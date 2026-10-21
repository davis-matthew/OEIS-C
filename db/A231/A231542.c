/*
 * A231542  Number of nX6 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A231542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: this function uses a transfer‑matrix DP over 6‑bit rows.
 * For each triple of consecutive rows (prev, cur, next) we precompute
 * whether the middle row cur satisfies the condition that every 0
 * in cur is not less than a strict majority of its existing neighbors
 * (horizontal, vertical, antidiagonal).  The DP iterates over rows
 * building the grid from top to bottom, keeping the last two rows.
 * For the final row we check the condition with no below neighbour.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* allowed neighbor count constants */
#define NEIGHBORS_MAX 6
#define SENTINEL 64          /* value outside 0..63 used for non‑existent rows */

static uint64_t transition_mask[65][65];   /* bits 0..63 for next row */
static bool      transition_last[65][65];  /* true if next row = SENTINEL is allowed */

/* ------------------------------------------------------------------ */
/*  helper: check whether a triple (prev,curr,next) satisfies the rule */
static bool check_triple(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        if (((curr >> j) & 1) == 0) {  /* only 0 cells impose a restriction */
            int ones  = 0;
            int total = 0;

            /* horizontal neighbours */
            if (j > 0) {
                ++total;
                if ( (curr >> (j - 1)) & 1 ) ++ones;
            }
            if (j < 5) {
                ++total;
                if ( (curr >> (j + 1)) & 1 ) ++ones;
            }

            /* vertical neighbours */
            if (prev != SENTINEL) {
                ++total;
                if ( (prev >> j) & 1 ) ++ones;
                if (j > 0) {            /* up‑left */
                    ++total;
                    if ( (prev >> (j - 1)) & 1 ) ++ones;
                }
            }

            if (next != SENTINEL) {
                ++total;
                if ( (next >> j) & 1 ) ++ones;
                if (j < 5) {            /* down‑right */
                    ++total;
                    if ( (next >> (j + 1)) & 1 ) ++ones;
                }
            }

            /* 0 must not be less than a strict majority of its neighbours */
            if (ones * 2 > total) return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  helper: initialise the transition tables only once               */
static void init_transition(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            uint64_t mask = 0;
            for (int next = 0; next <= SENTINEL; ++next) {
                bool ok = check_triple(prev, curr, next);
                if (next == SENTINEL) {
                    transition_last[prev][curr] = ok;
                } else if (ok) {
                    mask |= ((uint64_t)1) << next;
                }
            }
            transition_mask[prev][curr] = mask;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  main sequence function                                            */
seq_t a(idx_t n)
{
    /* n >= 1 */
    init_transition();

    /* dp[prev][curr] holds the count after processing some rows,
       where prev is the row before curr, curr is the last row. */
    static seq_t dp[65][65];
    static seq_t dp_next[65][65];

    /* initialise dp for the first row: prev = SENTINEL */
    for (int prev = 0; prev <= SENTINEL; ++prev)
        for (int curr = 0; curr < 64; ++curr)
            dp[prev][curr] = 0;
    for (int curr = 0; curr < 64; ++curr)
        dp[SENTINEL][curr] = 1;

    /* iterate over remaining rows (if any) */
    for (idx_t row = 1; row < n; ++row) {
        /* clear dp_next */
        for (int prev = 0; prev <= SENTINEL; ++prev)
            for (int curr = 0; curr < 64; ++curr)
                dp_next[prev][curr] = 0;

        /* transfer */
        for (int prev = 0; prev <= SENTINEL; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (cnt == 0) continue;

                uint64_t mask = transition_mask[prev][curr];
                /* iterate over set bits: all admissible next rows */
                while (mask) {
                    uint64_t lsb  = mask & -mask;
                    int next = __builtin_ctzll(mask);
                    mask ^= lsb;
                    dp_next[curr][next] += cnt;
                }
            }
        }
        /* move dp_next to dp */
        for (int prev = 0; prev <= SENTINEL; ++prev)
            for (int curr = 0; curr < 64; ++curr)
                dp[prev][curr] = dp_next[prev][curr];
    }

    /* finalisation: add transition to SENTINEL (no below row) */
    seq_t result = 0;
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            if (!dp[prev][curr]) continue;
            if (transition_last[prev][curr]) {
                result += dp[prev][curr];
            }
        }
    }
    return result;
}
