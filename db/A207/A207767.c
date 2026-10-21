/*
 * A207767  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays avoiding * horizontal
 *            patterns 000 and 101 and vertical patterns 011 and 101. * *
 *            Implementation uses a transfer‑matrix DP with state equal to the
 *            * last two column bit patterns (7 bits each). The set of allowed
 *            * column patterns is filtered first, reducing the...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   _valid_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_valid_column.h"

#define MAX_COLS 128
#define MAX_NEXT 32

/* Pre‑computed tables – initialised on first use. */
static bool _initialized = false;

/* Number of admissible column patterns. */
static idx_t _valid_count;

/* Mapping from mask (0..127) to its index in valid lists; -1 if invalid. */
static signed char _mask_index[MAX_COLS];

/* Valid column masks indexed by 0.._valid_count-1. */
static unsigned char _valid_mask[MAX_COLS];

/* For each ordered pair (i,j) of valid columns, the list of
 * admissible next columns.  _next_cnt[i][j] contains the number
 * of entries, and _next_alt[i][j][k] the k‑th admissible index. */
static unsigned char _next_cnt[MAX_COLS][MAX_COLS];
static unsigned char _next_alt[MAX_COLS][MAX_COLS][MAX_NEXT];

/* DP tables reused for each call. */
static seq_t _dp[MAX_COLS][MAX_COLS];
static seq_t _dp_next[MAX_COLS][MAX_COLS];

/* Initialise all static tables (once). */
static void _init_tables(void)
{
    if (_initialized) return;

    /* Build list of valid column masks. */
    _valid_count = 0;
    for (int m = 0; m < (1 << 7); ++m) {
        if (_valid_column((unsigned char)m)) {
            _mask_index[m] = (signed char)_valid_count;
            _valid_mask[_valid_count++] = (unsigned char)m;
        } else {
            _mask_index[m] = -1;
        }
    }

    /* Pre‑compute admissible third columns for every pair (a,b). */
    for (unsigned char a = 0; a < _valid_count; ++a) {
        for (unsigned char b = 0; b < _valid_count; ++b) {
            unsigned char *next_list = _next_alt[a][b];
            unsigned char cnt = 0;
            for (unsigned char c = 0; c < _valid_count; ++c) {
                bool ok = true;
                for (int r = 0; r < 7; ++r) {
                    unsigned char ba = (_valid_mask[a] >> r) & 1u;
                    unsigned char bb = (_valid_mask[b] >> r) & 1u;
                    unsigned char bc = (_valid_mask[c] >> r) & 1u;
                    if ((ba == 0 && bb == 0 && bc == 0) ||      /* 000 */
                        (ba == 1 && bb == 0 && bc == 1)) {       /* 101 */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    next_list[cnt++] = c;
                    if (cnt == MAX_NEXT) break; /* safety, should not happen */
                }
            }
            _next_cnt[a][b] = cnt;
        }
    }

    _initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    _init_tables();

    if (n == 0) return (seq_t)0;          /* undefined below offset */

    /* Base cases. */
    if (n == 1) return (seq_t)_valid_count;
    if (n == 2) return (seq_t)_valid_count * (seq_t)_valid_count;

    /* Initialise DP for the first two columns: all pairs allowed. */
    for (unsigned char i = 0; i < _valid_count; ++i) {
        for (unsigned char j = 0; j < _valid_count; ++j) {
            _dp[i][j] = (seq_t)1;
        }
    }

    /* Iterate over remaining columns. */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero the next table */
        for (unsigned char i = 0; i < _valid_count; ++i)
            for (unsigned char j = 0; j < _valid_count; ++j)
                _dp_next[i][j] = 0;
        /* transition */
        for (unsigned char a = 0; a < _valid_count; ++a) {
            for (unsigned char b = 0; b < _valid_count; ++b) {
                seq_t v = _dp[a][b];
                if (!v) continue;
                unsigned char cnt = _next_cnt[a][b];
                unsigned char *list = _next_alt[a][b];
                for (unsigned char k = 0; k < cnt; ++k) {
                    _dp_next[b][list[k]] += v;
                }
            }
        }
        /* copy back to _dp */
        for (unsigned char i = 0; i < _valid_count; ++i)
            for (unsigned char j = 0; j < _valid_count; ++j)
                _dp[i][j] = _dp_next[i][j];
    }

    /* Sum all ending states to obtain the final count. */
    seq_t ans = 0;
    for (unsigned char i = 0; i < _valid_count; ++i)
        for (unsigned char j = 0; j < _valid_count; ++j)
            ans += _dp[i][j];

    return ans;
}
