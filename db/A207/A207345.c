/*
 * A207345  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over allowed rows, avoiding
   000 and 011 horizontally and 000/111 vertically. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 7

/* Helper to get bit j of a 7‑bit mask */
static inline uint8_t bit_at(uint8_t m, int j) { return (m >> j) & 1u; }

/* Static tables (built once) */
static bool initialized = false;
static int allowed_cnt = 0;
static uint8_t *allowed_rows = NULL;
static uint16_t **adj = NULL;        /* adjacency: from state s to allowed next rows */
static size_t *adj_len = NULL;       /* number of successors for each state */
static size_t state_cnt = 0;

/* DP buffers reused across calls */
static seq_t *dp_a = NULL;
static seq_t *dp_b = NULL;

static void init_tables(void)
{
    /* Build list of allowed rows (no 000 or 011 horizontally) */
    uint8_t rows[128];
    int idx[128];
    for (int i = 0; i < 128; ++i) idx[i] = -1;
    allowed_cnt = 0;
    for (int m = 0; m < 128; ++m) {
        bool ok = true;
        for (int j = 0; j <= 4 && ok; ++j) {
            uint8_t b = (m >> j) & 7u;
            if (b == 0 || b == 0b011) ok = false;
        }
        if (ok) {
            idx[m] = allowed_cnt;
            rows[allowed_cnt] = (uint8_t)m;
            ++allowed_cnt;
        }
    }
    allowed_rows = (uint8_t*)malloc(allowed_cnt * sizeof(uint8_t));
    for (int i = 0; i < allowed_cnt; ++i) allowed_rows[i] = rows[i];

    /* Build state space and adjacency lists */
    state_cnt = (size_t)allowed_cnt * (size_t)allowed_cnt;
    adj   = (uint16_t**)malloc(state_cnt * sizeof(uint16_t*));
    adj_len = (size_t*)malloc(state_cnt * sizeof(size_t));
    for (size_t s = 0; s < state_cnt; ++s) {
        adj[s] = (uint16_t*)malloc(allowed_cnt * sizeof(uint16_t));
        adj_len[s] = 0;
    }

    for (size_t s = 0; s < state_cnt; ++s) {
        int x_idx = (int)(s / (size_t)allowed_cnt);
        int y_idx = (int)(s % (size_t)allowed_cnt);
        uint8_t x = allowed_rows[x_idx];
        uint8_t y = allowed_rows[y_idx];

        for (int z_idx = 0; z_idx < allowed_cnt; ++z_idx) {
            uint8_t z = allowed_rows[z_idx];
            bool ok = true;
            for (int j = 0; j < WIDTH && ok; ++j) {
                if (bit_at(x,j) == bit_at(y,j) && bit_at(y,j) == bit_at(z,j))
                    ok = false;
            }
            if (ok) {
                adj[s][adj_len[s]++] = (uint16_t)z_idx;
            }
        }
    }

    /* Allocate DP buffers */
    dp_a = (seq_t*)malloc(state_cnt * sizeof(seq_t));
    dp_b = (seq_t*)malloc(state_cnt * sizeof(seq_t));

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    if (n == 0) return 0;
    if (n == 1) return (seq_t)allowed_cnt;
    if (n == 2) {
        seq_t t = (seq_t)allowed_cnt;
        return t * t;
    }

    /* DP for height = 2: every pair of allowed rows is valid */
    for (size_t i = 0; i < state_cnt; ++i) dp_a[i] = 1;

    for (idx_t h = 3; h <= n; ++h) {
        /* zero next buffer */
        for (size_t i = 0; i < state_cnt; ++i) dp_b[i] = 0;

        for (size_t s = 0; s < state_cnt; ++s) {
            seq_t val = dp_a[s];
            if (val == 0) continue;
            size_t len = adj_len[s];
            for (size_t k = 0; k < len; ++k) {
                uint16_t z_idx = adj[s][k];
                size_t ns = (size_t)(s % (size_t)allowed_cnt) * (size_t)allowed_cnt + z_idx;
                dp_b[ns] += val;
            }
        }
        /* swap dp_a and dp_b for next iteration */
        seq_t *tmp = dp_a;
        dp_a = dp_b;
        dp_b = tmp;
    }

    /* Sum all states for the final height */
    seq_t result = 0;
    for (size_t i = 0; i < state_cnt; ++i) result += dp_a[i];
    return result;
}
