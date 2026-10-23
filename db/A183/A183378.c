/*
 * A183378  Number of nX5 binary arrays with no element equal to the mod 3 sum
 *          of its king-move neighbors.
 * https://oeis.org/A183378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   check_row_masks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: DP over rows with a transfer matrix that enforces
   the king‑move neighbor constraint for a 5‑column binary array. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/check_row_masks.h"

static uint32_t trans_mid[32][32];      /* next masks allowed when both prev & next present */
static uint32_t trans_prev_absent[32];  /* next masks when prev absent, next present */
static bool valid_last[32][32];         /* cur valid when next absent given prev present */
static bool valid_single_row[32];       /* cur valid when both prev & next absent (n==1) */
static bool tables_initialized = false;

static void init_tables(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            uint32_t mask = 0;
            for (int next = 0; next < 32; ++next) {
                if (check_row_masks((uint32_t)prev, true,
                                    (uint32_t)cur, true,
                                    (uint32_t)next)) {
                    mask |= (1u << next);
                }
            }
            trans_mid[prev][cur] = mask;
        }
    }

    for (int cur = 0; cur < 32; ++cur) {
        uint32_t mask = 0;
        for (int next = 0; next < 32; ++next) {
            if (check_row_masks(0, false,
                                (uint32_t)cur, true,
                                (uint32_t)next)) {
                mask |= (1u << next);
            }
        }
        trans_prev_absent[cur] = mask;
    }

    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            valid_last[prev][cur] =
                check_row_masks((uint32_t)prev, true,
                                (uint32_t)cur, false, 0);
        }
    }

    for (int cur = 0; cur < 32; ++cur) {
        valid_single_row[cur] =
            check_row_masks(0, false, (uint32_t)cur, false, 0);
    }

    tables_initialized = true;
}

seq_t a(idx_t n)
{
    if (!tables_initialized) init_tables();

    if (n == 0) return (seq_t)0; /* unused, offset starts at 1 */

    if (n == 1) {
        seq_t cnt = 0;
        for (int cur = 0; cur < 32; ++cur)
            if (valid_single_row[cur]) ++cnt;
        return cnt;
    }

    /* dp[prev][cur] */
    seq_t dp[32][32] = {0};
    seq_t next_dp[32][32] = {0};

    /* initialise with first two rows */
    for (int first = 0; first < 32; ++first) {
        uint32_t mask = trans_prev_absent[first];
        for (int second = 0; second < 32; ++second) {
            if (mask & (1u << second))
                dp[first][second] = (seq_t)1;
        }
    }

    /* process remaining rows, from row 2 to row n-1 */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next_dp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                uint32_t mask = trans_mid[prev][cur];
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mask & (1u << nxt))
                        next_dp[cur][nxt] += ways;
                }
            }
        }

        /* copy next_dp to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* final row must satisfy constraints with next absent */
    seq_t total = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            if (valid_last[prev][cur]) {
                total += dp[prev][cur];
            }
        }
    }
    return total;
}
