/*
 * A209223  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A209223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 21, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209223: number of n×7 binary arrays avoiding
   horizontal patterns 000 and 010, vertical patterns 010 and 111.
   Computed by a transfer‑matrix DP over pairs of rows. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The types seq_t and idx_t are defined by the build. */
static uint8_t _valid_rows[128];
static unsigned char _cnt_next[128*128];
static int _next_list[128*128][128];
static size_t _R = 0;
static seq_t *_cur = NULL, *_next = NULL;

/* Initialise static tables only once. */
static void _init_tables()
{
    if (_R != 0) return;
    /* Compute all rows that avoid horizontal 000 and 010. */
    size_t r = 0;
    for (int v = 0; v < 128; ++v) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
            int t = ((v >> i) & 1) << 2 |
                    ((v >> (i + 1)) & 1) << 1 |
                    ((v >> (i + 2)) & 1);
            if (t == 0b000 || t == 0b010) { ok = false; break; }
        }
        if (ok) _valid_rows[r++] = (uint8_t)v;
    }
    _R = r;

    /* Build adjacency lists: for each pair (p,q) store all r that are allowed
       vertically with them. */
    for (size_t i = 0; i < _R; ++i) {
        for (size_t j = 0; j < _R; ++j) {
            unsigned char cnt = 0;
            size_t base = i * 128 + j;   /* use 128 fixed stride for static array */
            for (size_t k = 0; k < _R; ++k) {
                bool ok = true;
                for (int c = 0; c < 7; ++c) {
                    int p = (_valid_rows[i] >> c) & 1;
                    int qb = (_valid_rows[j] >> c) & 1;
                    int r_bit = (_valid_rows[k] >> c) & 1;
                    if ((p == 0 && qb == 1 && r_bit == 0) ||
                        (p == 1 && qb == 1 && r_bit == 1)) { ok = false; break; }
                }
                if (ok) _next_list[base][cnt++] = (int)k;
            }
            _cnt_next[base] = cnt;
        }
    }

    /* Allocate DP arrays. */
    _cur  = (seq_t *)malloc(sizeof(seq_t) * _R * _R);
    _next = (seq_t *)malloc(sizeof(seq_t) * _R * _R);
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined behavior for idx<1 */
    _init_tables();

    /* Base cases. */
    if (n == 1) return (seq_t)_R;
    if (n == 2) return (seq_t)_R * (seq_t)_R;

    /* Initialise counting for two first rows: all pairs are allowed. */
    size_t R = _R;
    for (size_t i = 0; i < R * R; ++i) _cur[i] = 1;

    /* Iterate over remaining rows. */
    idx_t steps = n - 2;
    for (idx_t step = 0; step < steps; ++step) {
        /* Zero next array. */
        for (size_t i = 0; i < R * R; ++i) _next[i] = 0;
        /* Transition from each state (i,j). */
        for (size_t i = 0; i < R; ++i) {
            for (size_t j = 0; j < R; ++j) {
                seq_t val = _cur[i * R + j];
                if (!val) continue;
                size_t base = i * 128 + j;   /* fixed stride used earlier */
                unsigned char cnt = _cnt_next[base];
                for (unsigned char idx2 = 0; idx2 < cnt; ++idx2) {
                    int k = _next_list[base][idx2];
                    _next[j * R + k] += val;
                }
            }
        }
        /* Swap pointers for next iteration. */
        seq_t *tmp = _cur; _cur = _next; _next = tmp;
    }

    /* Sum all states to obtain the final count. */
    seq_t ans = 0;
    for (size_t i = 0; i < R * R; ++i) ans += _cur[i];
    return ans;
}
