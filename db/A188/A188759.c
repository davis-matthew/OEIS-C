/*
 * A188759  Number of nX5 binary arrays without the pattern 0 0 1 vertically
 *          or horizontally.
 * https://oeis.org/A188759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding a * 0‑0‑1 pattern
 *            vertically or horizontally. * The function counts all such
 *            arrays directly by dynamic programming
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Pre‑computation of all allowed 5‑bit rows and transitions.       */
/* ------------------------------------------------------------------ */
struct {
    bool inited;
    /* mapping from a 5‑bit mask to index in the list of valid rows.
       0xFF indicates the mask is forbidden (contains 0b001). */
    uint8_t map[32];
    /* list of all valid row masks. */
    uint8_t rows[32];
    uint8_t cnt;        /* number of valid rows */
    seq_t  cnt_rows;    /* same value as seq_t                               */
    /* number of allowed successor rows for a pair (a,b) */
    uint8_t next_len[32 * 32];
    /* successor masks for each pair */
    uint8_t next_rows[32 * 32][32];
    /* DP arrays used for a single evaluation. */
    seq_t dp[32 * 32];
    seq_t dp_next[32 * 32];
} _a188759;

static void _a188759_init(void)
{
    if (_a188759.inited) return;
    _a188759.inited = true;

    /* 1.  Find all 5‑bit rows that do NOT contain the substring 001. */
    _a188759.cnt = 0;
    for (uint32_t mask = 0; mask < 32; ++mask) {
        bool ok = true;
        for (int i = 0; i <= 2; ++i) {
            if ( ((mask >> i) & 0x7u) == 1u ) { /* 0b001 */
                ok = false;
                break;
            }
        }
        if (ok) {
            _a188759.rows[_a188759.cnt] = (uint8_t)mask;
            _a188759.map[mask]        = _a188759.cnt;
            ++_a188759.cnt;
        } else {
            _a188759.map[mask] = 0xFF;
        }
    }
    _a188759.cnt_rows = (seq_t)_a188759.cnt;

    /* 2.  Pre‑compute transitions for every pair of rows. */
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            int idx = a * 32 + b;
            if (_a188759.map[a] == 0xFF || _a188759.map[b] == 0xFF) {
                _a188759.next_len[idx] = 0;
                continue;
            }
            /* Positions where a and b both have 0 must not be 1 in the next row. */
            uint32_t forbid = (~a & ~b) & 0x1Fu;
            uint8_t len = 0;
            for (uint32_t c = 0; c < 32; ++c) {
                if (_a188759.map[c] == 0xFF) continue;
                if ( (c & forbid) == 0 ) {
                    _a188759.next_rows[idx][len++] = (uint8_t)c;
                }
            }
            _a188759.next_len[idx] = len;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Count arrays of size n×5.                                          */
/* ------------------------------------------------------------------ */
static void _a188759_compute(seq_t *res, idx_t n)
{
    _a188759_init();

    if (n == 1) {
        *res = _a188759.cnt_rows;
        return;
    }
    if (n == 2) {
        *res = _a188759.cnt_rows * _a188759.cnt_rows;
        return;
    }

    seq_t *curr  = _a188759.dp;
    seq_t *next  = _a188759.dp_next;

    /* initialise DP for column count 2: one for each pair of valid rows */
    for (int i = 0; i < 32 * 32; ++i) curr[i] = 0;
    for (int a = 0; a < 32; ++a) {
        if (_a188759.map[a] == 0xFF) continue;
        for (int b = 0; b < 32; ++b) {
            if (_a188759.map[b] == 0xFF) continue;
            curr[a * 32 + b] = 1;
        }
    }

    /* iterate for columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 32 * 32; ++i) next[i] = 0;
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                int idx = a * 32 + b;
                seq_t ways = curr[idx];
                if (ways == 0) continue;
                uint8_t len = _a188759.next_len[idx];
                const uint8_t *list = _a188759.next_rows[idx];
                for (int t = 0; t < len; ++t) {
                    int new_idx = b * 32 + list[t];
                    next[new_idx] += ways;
                }
            }
        }
        /* swap current and next */
        seq_t *tmp = curr; curr = next; next = tmp;
    }

    /* sum all ways for the final column */
    seq_t sum = 0;
    for (int i = 0; i < 32 * 32; ++i) sum += curr[i];
    *res = sum;
}

/* ------------------------------------------------------------------ */
/*  Public API.                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t r;
    _a188759_compute(&r, n);
    return r;
}
