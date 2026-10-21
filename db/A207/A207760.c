/*
 * A207760  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   has_forbidden_horizontal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207760: number of n×6 binary arrays avoiding the patterns
   horizontally 000 and 101, and vertically 011 and 101.
   Computed by dynamic programming over valid row patterns. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/has_forbidden_horizontal.h"

/* Precomputed data shared among calls */
static int  valid_rows[64];
static int  row_to_idx[64];
static int  m = -1;          /* number of valid rows */
static int *next_states_list[4096];
static int  next_states_count[4096];
static bool transitions_precomputed = false;

/* Count arrays for a given n */
seq_t a(idx_t n) {
    /* Build the list of valid rows once. */
    if (m < 0) {
        int count = 0;
        for (int r = 0; r < 64; ++r) {
            if (!has_forbidden_horizontal(r)) {
                valid_rows[count] = r;
                row_to_idx[r] = count;
                ++count;
            }
        }
        m = count;              /* m == 19 for 6 columns */
    }

    /* Trivial cases. */
    if (n == 1) return (seq_t)m;                     /* 19 */
    if (n == 2) return (seq_t)m * (seq_t)m;           /* 361 */

    /* Number of states = m*m <= 361. */
    const int M = m * m;

    /* Build transition lists once. */
    if (!transitions_precomputed) {
        for (int i = 0; i < M; ++i) {
            int p = i / m;
            int q = i % m;
            int a = valid_rows[p];
            int b = valid_rows[q];
            int temp[20];          /* m <= 19 */
            int cnt = 0;
            for (int r = 0; r < m; ++r) {
                int c = valid_rows[r];
                bool bad = false;
                for (int col = 0; col < 6; ++col) {
                    int abit = (a >> col) & 1;
                    int bbit = (b >> col) & 1;
                    int cbit = (c >> col) & 1;
                    if ((abit == 0 && bbit == 1 && cbit == 1) ||
                        (abit == 1 && bbit == 0 && cbit == 1)) {
                        bad = true;
                        break;
                    }
                }
                if (!bad) {
                    temp[cnt++] = q * m + r;   /* next state index */
                }
            }
            if (cnt) {
                next_states_list[i] = (int*)malloc(cnt * sizeof(int));
                memcpy(next_states_list[i], temp, cnt * sizeof(int));
                next_states_count[i] = cnt;
            } else {
                next_states_list[i] = NULL;
                next_states_count[i] = 0;
            }
        }
        transitions_precomputed = true;
    }

    /* Dynamic programming arrays. */
    seq_t *cur  = (seq_t*)calloc(M, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(M, sizeof(seq_t));
    for (int i = 0; i < M; ++i)
        cur[i] = (seq_t)1;                 /* all sequences of length 2 */

    for (idx_t step = 3; step <= n; ++step) {
        memset(next, 0, M * sizeof(seq_t));

        for (int i = 0; i < M; ++i) {
            seq_t val = cur[i];
            if (!val) continue;
            int cnt = next_states_count[i];
            int *list = next_states_list[i];
            for (int k = 0; k < cnt; ++k) {
                int j = list[k];
                next[j] += val;
            }
        }

        /* Swap arrays. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all sequences of length n. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < M; ++i)
        total += cur[i];

    free(cur);
    free(next);
    return total;
}
