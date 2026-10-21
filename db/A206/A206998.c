/*
 * A206998  Number of nX6 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by a transfer-matrix DP counting n×6 binary
 *            arrays that avoid horizontal pattern 010 and vertical pattern
 *            011. For n = 1: a(1) = 37 For n > 1: a(n) = Σ_{r_{n-1},r_n}
 *            DP[n](r_{n-1},r_n) The DP is performed with states = 37×37 =
 *            1369, where each state encodes two consecutive rows....
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int _a206998_initialized = 0;

/* counts of valid 6-bit rows and their patterns */
static int _rows = 0;
static unsigned char _row_pattern[64];

/* transition data */
static size_t _states = 0;
static int    *_next_count = NULL;         /* number of transitions from a state */
static int    *_next_idx   = NULL;         /* compact array of destination states */

/* DP storage for the current and next row */
static seq_t *_dp_cur  = NULL;
static seq_t *_dp_next = NULL;

/* helper to generate all valid 6-bit rows */
static void _init_rows(void)
{
    int i, j;
    for (int mask = 0; mask < 64; ++mask) {
        int bad = 0;
        for (j = 0; j <= 3; ++j) {
            int b0 = (mask >> j)     & 1;
            int b1 = (mask >> (j+1)) & 1;
            int b2 = (mask >> (j+2)) & 1;
            if (b0 == 0 && b1 == 1 && b2 == 0) {
                bad = 1;
                break;
            }
        }
        if (!bad) {
            _row_pattern[_rows] = (unsigned char)mask;
            ++_rows;
        }
    }
}

/* initialise all tables */
static void _init_sequence(void)
{
    if (_a206998_initialized) return;
    _init_rows();
    _states = (size_t)_rows * (size_t)_rows;

    /* allocate DP arrays */
    _dp_cur  = (seq_t*)calloc(_states, sizeof(seq_t));
    _dp_next = (seq_t*)calloc(_states, sizeof(seq_t));

    /* allocate transition tables */
    _next_count = (int*)calloc(_states, sizeof(int));
    _next_idx   = (int*)calloc(_states * _rows, sizeof(int));

    /* build transitions */
    for (int i1 = 0; i1 < _rows; ++i1) {
        unsigned char r1 = _row_pattern[i1];
        unsigned char inv_r1 = (unsigned char)((~r1) & 0x3F);
        for (int i2 = 0; i2 < _rows; ++i2) {
            unsigned char r2 = _row_pattern[i2];
            size_t s_index = (size_t)i1 * (size_t)_rows + (size_t)i2;
            int cnt = 0;
            for (int k = 0; k < _rows; ++k) {
                unsigned char r3 = _row_pattern[k];
                if (((inv_r1 & r2 & r3) & 0x3F) == 0) {
                    _next_idx[s_index * _rows + cnt] = k;
                    ++cnt;
                }
            }
            _next_count[s_index] = cnt;
        }
    }

    _a206998_initialized = 1;
}

seq_t a(idx_t n)
{
    _init_sequence();

    if (n == 0) return 0; /* undefined in OEIS, safe return */

    /* For n = 1 and 2, use direct formulas */
    if (n == 1) return (seq_t)_rows;
    if (n == 2) return (seq_t)_rows * (seq_t)_rows;

    /* initialise DP for two first rows: one way per state */
    for (size_t s = 0; s < _states; ++s)
        _dp_cur[s] = 1;

    /* DP for rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next DP */
        memset(_dp_next, 0, _states * sizeof(seq_t));

        for (size_t s = 0; s < _states; ++s) {
            seq_t val = _dp_cur[s];
            if (val == 0) continue;

            int cnt = _next_count[s];
            int i2  = (int)(s % _rows);      /* second row of the state */

            size_t base = s * (size_t)_rows;
            for (int t = 0; t < cnt; ++t) {
                int k = _next_idx[base + t];
                size_t new_s = (size_t)i2 * (size_t)_rows + (size_t)k;
                _dp_next[new_s] += val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = _dp_cur;
        _dp_cur = _dp_next;
        _dp_next = tmp;
    }

    /* sum all end states */
    seq_t result = 0;
    for (size_t s = 0; s < _states; ++s)
        result += _dp_cur[s];

    return result;
}
