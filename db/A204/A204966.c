/*
 * A204966  Number of (n+2)X5 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally at least two different ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count (n+2)×5 binary arrays whose every 3×3 subblock contains at
 *            least two distinct “triple‑equals” lines (horizontal, vertical,
 *            main or anti diagonal). Arrays are enumerated row‑major with the
 *            first entry forced to 0
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 202 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline bool _check_window(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t c)
{
    bool b[3][3];
    uint8_t rows[3] = { r1, r2, r3 };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            b[i][j] = (rows[i] >> (c + j)) & 1U;
        }
    }

    int lines = 0;
    for (int i = 0; i < 3; ++i)
        if (b[i][0] == b[i][1] && b[i][1] == b[i][2]) ++lines;
    for (int j = 0; j < 3; ++j)
        if (b[0][j] == b[1][j] && b[1][j] == b[2][j]) ++lines;
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) ++lines;
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) ++lines;
    return lines >= 2;
}

static bool _valid[32][32][32];
static uint8_t _next_row[32][32][32];
static uint8_t _next_cnt[32][32];
static bool _tables_inited = false;

static void _init_tables(void)
{
    if (_tables_inited) return;
    for (uint8_t r1 = 0; r1 < 32; ++r1) {
        for (uint8_t r2 = 0; r2 < 32; ++r2) {
            uint8_t cnt = 0;
            for (uint8_t r3 = 0; r3 < 32; ++r3) {
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    if (!_check_window(r1, r2, r3, c)) { ok = false; break; }
                }
                _valid[r1][r2][r3] = ok;
                if (ok) _next_row[r1][r2][cnt++] = r3;
            }
            _next_cnt[r1][r2] = cnt;
        }
    }
    _tables_inited = true;
}

seq_t a(idx_t n)
{
    _init_tables();

    seq_t dp_curr[32][32];
    seq_t dp_next[32][32];

    for (uint8_t r1 = 0; r1 < 32; ++r1) {
        for (uint8_t r2 = 0; r2 < 32; ++r2)
            dp_curr[r1][r2] = ((r1 & 1u) == 0) ? (seq_t)1 : (seq_t)0;
    }

    for (idx_t step = 0; step < n; ++step) {
        for (uint8_t r1 = 0; r1 < 32; ++r1)
            for (uint8_t r2 = 0; r2 < 32; ++r2)
                dp_next[r1][r2] = (seq_t)0;

        for (uint8_t r1 = 0; r1 < 32; ++r1) {
            for (uint8_t r2 = 0; r2 < 32; ++r2) {
                seq_t val = dp_curr[r1][r2];
                if (val == (seq_t)0) continue;
                uint8_t cnt = _next_cnt[r1][r2];
                const uint8_t *list = _next_row[r1][r2];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t r3 = list[k];
                    dp_next[r2][r3] += val;
                }
            }
        }

        for (uint8_t r1 = 0; r1 < 32; ++r1)
            for (uint8_t r2 = 0; r2 < 32; ++r2)
                dp_curr[r1][r2] = dp_next[r1][r2];
    }

    seq_t result = (seq_t)0;
    for (uint8_t r1 = 0; r1 < 32; ++r1)
        for (uint8_t r2 = 0; r2 < 32; ++r2)
            result += dp_curr[r1][r2];

    return result;
}
