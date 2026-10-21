/*
 * A203404  Number of (n+2)X7 binary arrays avoiding patterns 000 and 111 in
 *          rows and columns.
 * https://oeis.org/A203404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A203404: DP over pairs of 7‑bit rows.
   Counts (n+2)×7 binary arrays avoiding 000 and 111 horizontally and
   vertically.  Uses 42 valid row patterns and transition graphs. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define ROWS 42
#define STATES (ROWS * ROWS)

static uint8_t row_patterns[ROWS];
static uint8_t adj_next[STATES][ROWS];
static uint8_t adj_cnt[STATES];
static bool adj_inited = false;

/* check horizontal 000/111 in a 7‑bit pattern */
static bool valid_row(uint8_t p)
{
    for (int col = 0; col <= 4; ++col) {
        int a = (p >> col)     & 1;
        int b = (p >> (col+1)) & 1;
        int c = (p >> (col+2)) & 1;
        if (a == b && b == c) return false;
    }
    return true;
}

/* check vertical 000/111 for three consecutive rows */
static bool valid_vertical(uint8_t a, uint8_t b, uint8_t c)
{
    for (int col = 0; col < 7; ++col) {
        int aa = (a >> col) & 1;
        int bb = (b >> col) & 1;
        int cc = (c >> col) & 1;
        if (aa == bb && bb == cc) return false;
    }
    return true;
}

/* initialise row patterns and adjacency lists */
static void init_adjacency(void)
{
    if (adj_inited) return;

    size_t idx = 0;
    for (uint8_t p = 0; p < 128; ++p) {
        if (valid_row(p)) {
            row_patterns[idx++] = p;
        }
    }

    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < ROWS; ++j) {
            size_t s = i * ROWS + j;
            adj_cnt[s] = 0;
            for (size_t k = 0; k < ROWS; ++k) {
                if (valid_vertical(row_patterns[i], row_patterns[j], row_patterns[k])) {
                    adj_next[s][adj_cnt[s]++] = (uint8_t)k;
                }
            }
        }
    }
    adj_inited = true;
}

/* dynamic programming arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static bool dp_inited = false;

static void init_dp_arrays(void)
{
    if (dp_inited) return;
    dp_cur  = (seq_t*)malloc(sizeof(seq_t) * STATES);
    dp_next = (seq_t*)malloc(sizeof(seq_t) * STATES);
    dp_inited = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence offset is 1 */
    init_adjacency();
    init_dp_arrays();

    /* start with any two rows – one for each of the 42×42 states */
    for (size_t s = 0; s < STATES; ++s)
        dp_cur[s] = 1;

    /* add rows 3 … (n+2): total of n iterations */
    for (idx_t step = 0; step < n; ++step) {
        for (size_t s = 0; s < STATES; ++s)
            dp_next[s] = 0;

        for (size_t s = 0; s < STATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == 0) continue;

            uint8_t i = (uint8_t)(s / ROWS);
            uint8_t j = (uint8_t)(s % ROWS);
            uint8_t tcnt = adj_cnt[s];
            for (uint8_t t = 0; t < tcnt; ++t) {
                uint8_t k = adj_next[s][t];
                size_t ns = (size_t)j * ROWS + k;
                dp_next[ns] += cnt;
            }
        }

        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* total number of arrays */
    seq_t total = 0;
    for (size_t s = 0; s < STATES; ++s)
        total += dp_cur[s];
    return total;
}
