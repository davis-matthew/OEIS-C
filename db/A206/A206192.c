/*
 * A206192  Number of (n+1)X4 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two.
 * https://oeis.org/A206192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   _allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A206192: Number of (n+1)x4 0..3 arrays with exactly two clockwise
 *  edge increases in every 2x2 subblock.
 *  The function uses a transfer‑matrix (DP) over the 256 possible rows.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_allowed.h"

static bool _adj_init = false;
static uint16_t _succ[256][256];
static uint16_t _deg[256];

static void _init_adj(void)
{
    for (int s = 0; s < 256; ++s)
    {
        uint16_t d = 0;
        for (int t = 0; t < 256; ++t)
            if (_allowed(s, t))
                _succ[s][d++] = (uint16_t)t;
        _deg[s] = d;
    }
    _adj_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* sequence is defined for n>=1, but guard */

    if (!_adj_init) _init_adj();

    /* DP vectors holding counts for current and next row */
    seq_t dp_cur[256];
    seq_t dp_next[256];

    /* Initially every first row is possible (value 1) */
    for (int i = 0; i < 256; ++i)
        dp_cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step)
    {
        /* clear dp_next */
        for (int i = 0; i < 256; ++i)
            dp_next[i] = (seq_t)0;

        for (int s = 0; s < 256; ++s)
        {
            seq_t val = dp_cur[s];
            /* if val is zero, skip (works for both builtin and big‑int types) */
            if (val == (seq_t)0) continue;

            uint16_t d = _deg[s];
            const uint16_t *succ = _succ[s];
            for (uint16_t k = 0; k < d; ++k)
                dp_next[succ[k]] = dp_next[succ[k]] + val;
        }

        /* swap dp_cur and dp_next for next iteration */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum over all final rows */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        result = result + dp_cur[i];

    return result;
}
