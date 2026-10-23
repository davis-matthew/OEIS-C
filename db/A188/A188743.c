/*
 * A188743  Number of nX5 binary arrays without the pattern 1 1 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   _valid_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements recurrence using DP over last two rows for a 5-column binary array. */
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_valid_transition.h"

/* Pre-computed transition masks: allowed next rows for each (prev2,prev1). */
static uint32_t _allow[32 * 32];
static bool _allow_init = false;

/* Initialise the transition table once. */
static void _init_allow(void)
{
    if (_allow_init) return;  /* already done */
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p1 = 0; p1 < 32; ++p1) {
            uint32_t mask = 0;
            for (int nxt = 0; nxt < 32; ++nxt) {
                if (_valid_transition(p2, p1, nxt))
                    mask |= (1u << nxt);
            }
            _allow[p2 * 32 + p1] = mask;
        }
    }
    _allow_init = true;
}

/* Main function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1; undefined below */
    if (n == 1) return 32;        /* any 5‑bit row */
    if (n == 2) return 1024;      /* any two rows */
    _init_allow();                /* ensure transitions ready */

    /* DP arrays: count[prev2][prev1] holds ways after current number of rows. */
    static seq_t count[32][32];
    static seq_t next[32][32];

    /* Base case for two rows:
       one way for each pair (prev2, prev1). */
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            count[i][j] = 1;
        }
    }

    /* Iterate rows 3 through n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero next array. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next[i][j] = 0;

        /* Transition. */
        for (int p2 = 0; p2 < 32; ++p2) {
            for (int p1 = 0; p1 < 32; ++p1) {
                seq_t val = count[p2][p1];
                if (val == 0) continue;
                uint32_t mask = _allow[p2 * 32 + p1];
                while (mask) {
                    int nxt = __builtin_ctz(mask);
                    mask &= mask - 1;
                    next[p1][nxt] += val;
                }
            }
        }

        /* Move to next row: copy next to count. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                count[i][j] = next[i][j];
    }

    /* Sum all counts after final row. */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            total += count[i][j];

    return total;
}
