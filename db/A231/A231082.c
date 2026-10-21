/*
 * A231082  Number of 3 X n 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4 and at least one element with value
 *          (x(i,j)-1) mod 4, and upper left element zero.
 * https://oeis.org/A231082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   _check_column, _decode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231082
   Formula: a(n) = number of 3×n arrays over {0,1,2,3} with top‑left entry 0
   and for every cell having at least one neighbour of value (x+1) mod 4
   and at least one neighbour of value (x−1) mod 4, where neighbours are
   horizontally or diagonally adjacent (vertical neighbours are NOT
   considered).  The values are obtained by a transfer‑matrix DP. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_check_column.h"
#include "../utils/_decode.h"

static bool _a231082_inited = false;

static uint8_t _mid_vals[64][3];
static bool _left_missing[64][64];
static bool _right_missing[64][64];
static bool _no_both[64];
static uint8_t _trans_next[64 * 64][64];
static uint8_t _trans_len[64 * 64];

/* initialise tables used by the DP */
static void _a231082_init(void) {
    for (uint8_t col = 0; col < 64; ++col)
        _decode(col, _mid_vals[col]);

    /* clear transition lengths */
    for (int i = 0; i < 64 * 64; ++i)
        _trans_len[i] = 0;

    /* transitions: middle column must be valid with both neighbours */
    for (uint8_t left_col = 0; left_col < 64; ++left_col) {
        for (uint8_t mid_col = 0; mid_col < 64; ++mid_col) {
            for (uint8_t right_col = 0; right_col < 64; ++right_col) {
                if (_check_column(mid_col, true, left_col, true, right_col)) {
                    uint16_t idx = (uint16_t)left_col * 64 + mid_col;
                    _trans_next[idx][_trans_len[idx]++] = right_col;
                }
            }
        }
    }

    /* left‑only and right‑only validity */
    for (uint8_t mid_col = 0; mid_col < 64; ++mid_col)
        for (uint8_t right_col = 0; right_col < 64; ++right_col)
            _left_missing[mid_col][right_col] =
                _check_column(mid_col, false, 0, true, right_col);

    for (uint8_t left_col = 0; left_col < 64; ++left_col)
        for (uint8_t mid_col = 0; mid_col < 64; ++mid_col)
            _right_missing[left_col][mid_col] =
                _check_column(mid_col, true, left_col, false, 0);

    /* single column with no neighbours */
    for (uint8_t mid_col = 0; mid_col < 64; ++mid_col)
        _no_both[mid_col] =
            _check_column(mid_col, false, 0, false, 0);

    _a231082_inited = true;
}

seq_t a(idx_t n) {
    if (!_a231082_inited) _a231082_init();

    /* base case n==1 */
    if (n == 1) {
        seq_t total = 0;
        for (uint8_t col = 0; col < 64; ++col) {
            if ((col & 3) != 0) continue;          /* top‑left must be 0 */
            if (_no_both[col]) total += 1;
        }
        return total;
    }

    /* DP arrays */
    static seq_t cur[64][64], nxt[64][64];
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            cur[i][j] = 0;

    /* initial pair of columns 1 and 2 */
    for (uint8_t c1 = 0; c1 < 64; ++c1) {
        if ((c1 & 3) != 0) continue;          /* top‑left must be 0 */
        for (uint8_t c2 = 0; c2 < 64; ++c2)
            if (_left_missing[c1][c2]) cur[c1][c2] += 1;
    }

    /* n==2 */
    if (n == 2) {
        seq_t total = 0;
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                if (cur[a][b] && _right_missing[a][b]) total += cur[a][b];
        return total;
    }

    /* columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                nxt[i][j] = 0;
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b) {
                seq_t ways = cur[a][b];
                if (!ways) continue;
                uint8_t len = _trans_len[a * 64 + b];
                for (uint8_t k = 0; k < len; ++k) {
                    uint8_t c = _trans_next[a * 64 + b][k];
                    nxt[b][c] += ways;
                }
            }
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* final column validation */
    seq_t total = 0;
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            if (cur[a][b] && _right_missing[a][b]) total += cur[a][b];
    return total;
}
