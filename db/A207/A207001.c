/*
 * A207001  Number of 4Xn 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A207001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP for the number of 4×N binary arrays avoiding horizontal 0‑1‑0
 *            and vertical 0‑1‑1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t types. */

static const uint8_t allowed_patterns[12] = {0,1,2,3,4,5,7,8,9,10,11,15};

static bool is_allowed[16] = {0};
static uint8_t bits[16][4];
static bool next_ok[16][16][12];

/* Initialise static data structures only once. */
static void init_static(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Prepare bits for all 16 patterns. */
    for (int pat = 0; pat < 16; ++pat) {
        for (int r = 0; r < 4; ++r)
            bits[pat][r] = (pat >> r) & 1U;
    }

    /* Mark allowed patterns. */
    for (int pat = 0; pat < 16; ++pat)
        is_allowed[pat] = false;
    for (int i = 0; i < 12; ++i)
        is_allowed[allowed_patterns[i]] = true;

    /* Precompute horizontal compatibility table. */
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int k = 0; k < 12; ++k) {
                uint8_t c = allowed_patterns[k];
                bool ok = true;
                for (int r = 0; r < 4; ++r) {
                    if (bits[a][r] == 0 && bits[b][r] == 1 && bits[c][r] == 0) {
                        ok = false;
                        break;
                    }
                }
                next_ok[a][b][k] = ok;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_static();

    const seq_t num_allowed = (seq_t)12;

    /* n = 1  →  one column, any allowed pattern */
    if (n == 1)
        return num_allowed;

    /* n = 2  →  any two allowed columns, no horizontal 0‑1‑0 */
    if (n == 2)
        return num_allowed * num_allowed;

    /* Dynamic programming: dp[a][b] = number of ways to fill columns
       up to current step with last two columns having patterns a and b. */
    seq_t dp[16][16] = {0};
    seq_t ndp[16][16] = {0};

    /* Initialise for the first two columns */
    for (int i = 0; i < 12; ++i) {
        uint8_t a_pat = allowed_patterns[i];
        for (int j = 0; j < 12; ++j) {
            uint8_t b_pat = allowed_patterns[j];
            dp[a_pat][b_pat] = (seq_t)1;
        }
    }

    /* Process remaining columns, from 3 up to n */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear ndp */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                ndp[a][b] = (seq_t)0;

        for (int a = 0; a < 16; ++a) {
            if (!is_allowed[a]) continue;
            for (int b = 0; b < 16; ++b) {
                if (!is_allowed[b]) continue;
                seq_t cnt = dp[a][b];
                if (cnt == (seq_t)0) continue;
                for (int k = 0; k < 12; ++k) {
                    if (!next_ok[a][b][k]) continue;
                    uint8_t c = allowed_patterns[k];
                    ndp[b][c] += cnt;
                }
            }
        }
        /* move ndp to dp */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum all ending states */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 16; ++a) {
        if (!is_allowed[a]) continue;
        for (int b = 0; b < 16; ++b) {
            if (!is_allowed[b]) continue;
            total += dp[a][b];
        }
    }
    return total;
}
