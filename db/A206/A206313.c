/*
 * A206313  Number of (n+1)X4 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A206313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with elements in {0,1,2,3} such
 *            that every 2×2 subblock has exactly two clockwise edge increases
 *            and a non‑zero determinant. The function is computed by dynamic
 *            programming on the 256 possible rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool _built = false;

/* 256 rows, each described by 4 base‑4 digits (0..3).  */
static uint8_t _row_digits[256][4];

/* adjacency lists: neighbours[i][k] = j for all rows j compatible
   with row i.  */
static uint8_t _neighbours[256][256];
static uint16_t _deg[256];

/* Precompute the adjacency structure.  */
static void _build(void)
{
    if (_built) return;
    for (uint16_t i = 0; i < 256; ++i) {
        _row_digits[i][0] = (i >> 6) & 3;
        _row_digits[i][1] = (i >> 4) & 3;
        _row_digits[i][2] = (i >> 2) & 3;
        _row_digits[i][3] = i & 3;
    }

    for (uint16_t i = 0; i < 256; ++i) {
        _deg[i] = 0;
        for (uint16_t j = 0; j < 256; ++j) {
            bool ok = true;
            /* check the three 2×2 blocks formed by rows i (top) and j (bottom) */
            for (int k = 0; k < 3 && ok; ++k) {
                uint8_t a = _row_digits[i][k];
                uint8_t b = _row_digits[i][k + 1];
                uint8_t c = _row_digits[j][k];
                uint8_t d = _row_digits[j][k + 1];

                int inc = 0;
                if (b > a) ++inc;
                if (d > b) ++inc;
                if (c > d) ++inc;
                if (a > c) ++inc;
                if (inc != 2) { ok = false; break; }

                if (a * d == b * c) { ok = false; break; }
            }
            if (ok) {
                _neighbours[i][_deg[i]++] = (uint8_t)j;
            }
        }
    }
    _built = true;
}

seq_t a(idx_t n)
{
    _build();

    /* number of rows = n+1.  We start with all possible first rows. */
    seq_t dp[256];
    seq_t newdp[256];

    for (uint16_t i = 0; i < 256; ++i)
        dp[i] = 0;
    for (uint16_t i = 0; i < 256; ++i)
        dp[i] = 1;

    for (idx_t step = 0; step < n; ++step) {   /* perform n transitions */
        for (uint16_t i = 0; i < 256; ++i)
            newdp[i] = 0;
        for (uint16_t i = 0; i < 256; ++i) {
            seq_t cnt = dp[i];
            if (cnt == 0) continue;
            for (uint16_t k = 0; k < _deg[i]; ++k) {
                uint8_t j = _neighbours[i][k];
                newdp[j] += cnt;
            }
        }
        for (uint16_t i = 0; i < 256; ++i)
            dp[i] = newdp[i];
    }

    seq_t total = 0;
    for (uint16_t i = 0; i < 256; ++i)
        total += dp[i];

    return total;
}
