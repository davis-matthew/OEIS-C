/*
 * A237102  Number of (n+1)X(2+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   _subblock_diff, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237102
   Formula: a(n) = number of (n+1)×3 arrays of 0‑2 where the
   difference (max – upper median) of every 2×2 subblock is the same
   for all subblocks. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/_subblock_diff.h"

/* --------------------------------------------------------------------- *
 *  For each pair of rows (i,j) we pre‑compute whether the two 2×2 subblocks
 *  formed by the row pair have equal values of 0‑2, each equal to the
 *  same difference d.  The result is stored in a 3‑by‑27‑by‑27 table.
 * --------------------------------------------------------------------- */
static bool _adj_done = false;
static bool _adj[3][27][27];

static void _build_adj(void)
{
    for (int d = 0; d < 3; ++d) {
        for (int i = 0; i < 27; ++i) {
            int a = i / 9;            /* row i, column 0 */
            int b = (i / 3) % 3;      /* row i, column 1 */
            int c = i % 3;            /* row i, column 2 */
            for (int j = 0; j < 27; ++j) {
                int x = j / 9;         /* next row, column 0 */
                int y = (j / 3) % 3;   /* next row, column 1 */
                int z = j % 3;         /* next row, column 2 */
                int diff0 = _subblock_diff(a, b, x, y);
                int diff1 = _subblock_diff(b, c, y, z);
                _adj[d][i][j] = (diff0 == d && diff1 == d);
            }
        }
    }
    _adj_done = true;
}

/* --------------------------------------------------------------------- *
 *  Memoisation of the sequence values and DP states.
 * --------------------------------------------------------------------- */
static seq_t * _cache = NULL;
static size_t  _cache_len = 0;
static size_t  _cache_cap = 0;

static seq_t _dp[3][27];
static seq_t _ndp[3][27];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, this value is unused */

    size_t idx = (size_t)n - 1;    /* zero‑based index into the cache */

    if (!_adj_done) _build_adj();

    /* initialise DP once */
    if (_cache_len == 0) {
        for (int d = 0; d < 3; ++d)
            for (int i = 0; i < 27; ++i)
                _dp[d][i] = 1;      /* one way for a single row */

        _cache_cap = 64;
        _cache = seq_alloc(_cache_cap);
    }

    /* generate sequence values up to the required index */
    while (_cache_len <= idx) {
        seq_t step_sum = 0;

        /* compute next DP layer */
        for (int d = 0; d < 3; ++d) {
            for (int j = 0; j < 27; ++j) _ndp[d][j] = 0;
            for (int i = 0; i < 27; ++i) {
                seq_t ways = _dp[d][i];
                if (!ways) continue;
                for (int j = 0; j < 27; ++j) {
                    if (_adj[d][i][j]) _ndp[d][j] += ways;
                }
            }
        }

        /* total number of arrays of current height (n+1 rows) */
        for (int d = 0; d < 3; ++d)
            for (int j = 0; j < 27; ++j)
                step_sum += _ndp[d][j];

        /* ensure cache capacity */
        if (_cache_len >= _cache_cap) {
            size_t new_cap = _cache_cap * 2;
            _cache = seq_realloc(_cache, _cache_cap, new_cap);
            _cache_cap = new_cap;
        }
        _cache[_cache_len++] = step_sum;

        /* prepare for the next iteration */
        for (int d = 0; d < 3; ++d)
            for (int i = 0; i < 27; ++i)
                _dp[d][i] = _ndp[d][i];
    }

    return _cache[idx];
}
