/*
 * A203360  Number of nX6 binary arrays with every 1 immediately preceded by 0
 *          to the left or above.
 * https://oeis.org/A203360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×6 binary arrays with every 1 immediately preceded by
 *            a 0 to the left or above. Computed via a row‑by‑row DP over the
 *            64 possible row masks. First row: bit 0 must be 0 and no two
 *            consecutive 1’s. For rows >1, a 1 at column j is allowed only if
 *            either the left neighbour (j–1) is 0 or th...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   first_row_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdbool.h>
#include "../utils/first_row_valid.h"

static bool allowed[64][64];
static bool allowed_init = false;

/* pre‑compute the transition table */
static void init_allowed(void) {
    if (allowed_init)
        return;
    for (unsigned int prev = 0; prev < 64; ++prev) {
        for (unsigned int cur = 0; cur < 64; ++cur) {
            bool ok = true;
            /* column 0 */
            if ((cur & 1) && (prev & 1))
                ok = false;
            /* columns 1..5 */
            for (int j = 1; j < 6 && ok; ++j) {
                if ((cur >> j) & 1) {      /* current bit is 1       */
                    if ((cur >> (j - 1)) & 1) {
                        if ((prev >> j) & 1)
                            ok = false;   /* left AND above are 1  */
                    }
                }
            }
            allowed[prev][cur] = ok;
        }
    }
    allowed_init = true;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;          /* offset 1 */

    init_allowed();        /* build transition once */

    static seq_t dp_cur[64];
    static seq_t dp_next[64];

    /* initialise for the first row */
    for (int m = 0; m < 64; ++m) {
        dp_cur[m] = first_row_valid((unsigned int)m) ? 1 : 0;
    }

    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 64; ++i)
            total += dp_cur[i];
        return total;
    }

    /* iterate rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < 64; ++i)
            dp_next[i] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            if (dp_cur[prev] == 0)
                continue;
            for (int cur = 0; cur < 64; ++cur) {
                if (allowed[prev][cur])
                    dp_next[cur] += dp_cur[prev];
            }
        }
        /* copy next to current */
        for (int i = 0; i < 64; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all rows */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        total += dp_cur[i];
    return total;
}
