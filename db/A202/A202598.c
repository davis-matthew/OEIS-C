/*
 * A202598  Number of (n+2)X7 binary arrays avoiding patterns 001 and 000 in
 *          rows and columns.
 * https://oeis.org/A202598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+2)×7 binary arrays avoiding the patterns 001 and
 *            000 in rows and columns. Computed with a transfer‑matrix DP over
 *            all admissible 7‑bit rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 7
static int K = 0;
static uint8_t mask_arr[128];
static bool zero_pair[128][128];
static bool init_flag = false;

static void init_tables(void)
{
    int cnt = 0;
    /* enumerate all 7‑bit masks that avoid 00 in positions 0‑4 */
    for (int m = 0; m < (1 << WIDTH); ++m) {
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            if ((((m >> i) & 1) == 0) && (((m >> (i + 1)) & 1) == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            mask_arr[cnt++] = (uint8_t)m;
        }
    }
    K = cnt;

    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            /* columns in which both rows have a 0 */
            uint8_t mask = (~mask_arr[i] & ~mask_arr[j]) & 0x7F;
            zero_pair[i][j] = (mask != 0);
        }
    }
    init_flag = true;
}

seq_t a(idx_t n)
{
    if (!init_flag) init_tables();

    idx_t h = n + 2;          /* number of rows */
    if (h == 0 || h == 1) {
        seq_t res = 0;
        for (int i = 0; i < K; ++i) res += (seq_t)1;
        return res;
    }
    if (h == 2) {
        seq_t res = 0;
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                res += (seq_t)1;
        return res;
    }

    size_t sz = (size_t)K * (size_t)K;
    seq_t *dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_prev || !dp_next) return (seq_t)0;  /* allocation failure */

    /* initialise for two rows: all pairs are possible */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            dp_prev[i * K + j] = (seq_t)1;

    size_t steps = h - 2;   /* number of rows to add beyond the first two */
    for (size_t step = 0; step < steps; ++step) {
        memset(dp_next, 0, sz * sizeof(seq_t));
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                seq_t val = dp_prev[i * K + j];
                if (!val) continue;
                if (zero_pair[i][j]) continue;  /* cannot extend further */
                for (int k = 0; k < K; ++k) {
                    dp_next[j * K + k] += val;
                }
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* sum all counts for the last row pair */
    seq_t result = 0;
    for (size_t idx = 0; idx < sz; ++idx) result += dp_prev[idx];

    free(dp_prev);
    free(dp_next);
    return result;
}
