/*
 * A208417  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 44, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208417: number of n×5 binary arrays avoiding horizontal 000 / 001
 *  and vertical 000 / 011.  The program builds all allowed rows
 *  (5‑bit masks) that contain no forbidden horizontal triple,
 *  precomputes a transition table for pairs of consecutive rows,
 *  and then performs a DP over the pairs of rows.
 *  Complexity: O(n · M²) where M ≤ 32 (M ≈ 13), easily below 2 s for
 *  n ≤ 10000.
 */

#include <stdint.h>
#include <stddef.h>

/* ==== static data for initialization ==== */
static int          _a208417_inited   = 0;
static int          _a208417_M;
static uint8_t      _a208417_allowed[32];
static uint32_t     _a208417_adj[1024];   /* M * M ≤ 1024 */
static seq_t        _a208417_dp0[1024];
static seq_t        _a208417_dp1[1024];
static seq_t *      _a208417_dp_prev;
static seq_t *      _a208417_dp_cur;

/* ==== helper to build the transition machinery ==== */
static void _a208417_prepare(void)
{
    int i, j, k, col;
    int M = 0;

    /* collect the allowed rows */
    for (i = 0; i < 32; ++i) {
        int ok = 1;
        for (col = 0; col <= 2 && ok; ++col) {
            int tri = (i >> col) & 0x7;
            if (tri == 0 || tri == 1) ok = 0;         /* 000 or 001 */
        }
        if (ok) {
            _a208417_allowed[M++] = (uint8_t)i;
        }
    }
    _a208417_M = M;

    /* build adjacency: from pair (prev2, prev1) to next row indices */
    for (i = 0; i < M; ++i) {
        for (j = 0; j < M; ++j) {
            uint32_t bits = 0;
            for (k = 0; k < M; ++k) {
                int ok = 1;
                for (col = 0; col < 5 && ok; ++col) {
                    int a_bit = (_a208417_allowed[i] >> col) & 1;
                    int b_bit = (_a208417_allowed[j] >> col) & 1;
                    int c_bit = (_a208417_allowed[k] >> col) & 1;
                    if (a_bit == 0 && b_bit == 0 && c_bit == 0) ok = 0;   /* 000 */
                    if (a_bit == 0 && b_bit == 1 && c_bit == 1) ok = 0;   /* 011 */
                }
                if (ok) bits |= (1u << k);
            }
            _a208417_adj[i * M + j] = bits;
        }
    }

    _a208417_dp_prev = _a208417_dp0;
    _a208417_dp_cur  = _a208417_dp1;
    _a208417_inited  = 1;
}


/* ==== main sequence function ==== */
seq_t a(idx_t n)
{
    if (!_a208417_inited) _a208417_prepare();

    if (n == 0) return (seq_t)0;                     /* undefined offset */

    if (n == 1) return (seq_t)_a208417_M;            /* any allowed row */
    if (n == 2) return (seq_t)_a208417_M * (seq_t)_a208417_M; /* any pair */

    size_t state_cnt = (size_t)_a208417_M * (size_t)_a208417_M;

    /* initialise DP for length 2 with count 1 for each pair */
    for (size_t s = 0; s < state_cnt; ++s) _a208417_dp_prev[s] = (seq_t)1;

    /* iterate rows 3 .. n */
    for (idx_t len = 3; len <= n; ++len) {
        /* clear next DP array */
        for (size_t s = 0; s < state_cnt; ++s) _a208417_dp_cur[s] = (seq_t)0;

        /* perform all transitions */
        for (size_t s = 0; s < state_cnt; ++s) {
            seq_t val = _a208417_dp_prev[s];
            if (!val) continue;
            uint32_t mask = _a208417_adj[s];
            size_t prev1 = s % (size_t)_a208417_M;      /* second row index */
            while (mask) {
                uint32_t low = mask & -mask;
                int k = __builtin_ctz(mask);
                mask -= low;
                size_t tgt = prev1 * (size_t)_a208417_M + (size_t)k;
                _a208417_dp_cur[tgt] += val;
            }
        }

        /* swap DP pointers for next iteration */
        seq_t *tmp = _a208417_dp_prev;
        _a208417_dp_prev = _a208417_dp_cur;
        _a208417_dp_cur  = tmp;
    }

    /* sum all counts for full length n */
    seq_t total = (seq_t)0;
    for (size_t s = 0; s < state_cnt; ++s) total += _a208417_dp_prev[s];
    return total;
}
