/*
 * A232080  Number of (4+1)X(n+1) 0..1 arrays with every element equal to some
 *          horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (4+1)x(n+1) binary arrays with each element
 *            equal to some horizontal, diagonal or antidiagonal neighbor,
 *            with top left element zero. The implementation uses dynamic
 *            programming over columns, keeping for each column a 5‑bit
 *            pattern and a 5‑bit “need” mask that indicates which rows...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define ROWS         5
#define PATMASK      0x1F          /* 5 bits for pattern      */
#define NEEDMASK     0x1F          /* 5 bits for need mask   */
#define STATES       1024          /* 32 patterns × 32 needs */
#define MAX_TRANS    32           /* at most 32 transitions per state */

/* forward declaration of a() */
seq_t a(idx_t n);

/* static transition table */
static uint16_t trans_table[STATES][MAX_TRANS];
static size_t   trans_count[STATES];
static bool     trans_prepared = false;

/* prepare transition table once */
static void
prepare_transitions(void)
{
    if (trans_prepared) return;
    for (int prev_pat = 0; prev_pat < 32; ++prev_pat) {
        for (int prev_need = 0; prev_need < 32; ++prev_need) {
            int state = (prev_pat << 5) | prev_need;
            size_t count = 0;
            for (int cur_pat = 0; cur_pat < 32; ++cur_pat) {
                /* Check that all unsatisfied rows in prev_pat get a match in cur_pat */
                bool ok = true;
                for (int r = 0; r < ROWS && ok; ++r) {
                    if ((prev_need >> r) & 1) {
                        int bitp = (prev_pat >> r) & 1;
                        bool match = false;
                        if (((cur_pat >> r) & 1) == bitp)                         match = true;
                        if (r > 0   && ((cur_pat >> (r - 1)) & 1) == bitp)       match = true;
                        if (r < ROWS - 1 && ((cur_pat >> (r + 1)) & 1) == bitp)   match = true;
                        if (!match) ok = false;
                    }
                }
                if (!ok) continue;

                /* Determine need mask for current column */
                int cur_need = 0;
                for (int r = 0; r < ROWS; ++r) {
                    int bitp = (prev_pat >> r) & 1;
                    bool sat = false;
                    if (((cur_pat >> r) & 1) == bitp)                         sat = true;
                    if (r > 0   && ((cur_pat >> r) & 1) == ((prev_pat >> (r - 1)) & 1)) sat = true;
                    if (r < ROWS - 1 && ((cur_pat >> r) & 1) == ((prev_pat >> (r + 1)) & 1)) sat = true;
                    if (!sat) cur_need |= (1 << r);
                }

                trans_table[state][count++] = (cur_pat << 5) | cur_need;
            }
            trans_count[state] = count;
        }
    }
    trans_prepared = true;
}

seq_t
a(idx_t n)
{
    prepare_transitions();

    /* DP arrays */
    seq_t dp[STATES]      = {0};
    seq_t next[STATES]    = {0};

    /* Initial column (index 0) */
    for (int pat = 0; pat < 32; ++pat) {
        if (pat & 1) continue;            /* top-left must be 0 (bit 0) */
        int state = (pat << 5) | NEEDMASK; /* all rows still need a right neighbor */
        dp[state] = 1;
    }

    /* Process columns 1..n */
    for (idx_t col = 1; col <= n; ++col) {
        for (int i = 0; i < STATES; ++i) next[i] = 0;
        for (int state = 0; state < STATES; ++state) {
            seq_t ways = dp[state];
            if (ways == 0) continue;
            size_t cnt = trans_count[state];
            for (size_t j = 0; j < cnt; ++j) {
                int nxt = trans_table[state][j];
                next[nxt] += ways;
            }
        }
        /* copy back */
        for (int i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* Sum states where the last column has no unsatisfied rows */
    seq_t result = 0;
    for (int state = 0; state < STATES; ++state) {
        if ((state & NEEDMASK) == 0)
            result += dp[state];
    }
    return result;
}
