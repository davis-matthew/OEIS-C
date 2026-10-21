/*
 * A207425  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Count 0/1 matrices n×7 avoiding horizontal 000/010 in each row and vertical 001/010 in each column.
   DP on pairs of rows (previous two rows) with precomputed admissible next rows. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define V_MAX 40
#define S_MAX (V_MAX*V_MAX)

static bool initialized = false;
static uint8_t valid_rows[V_MAX];
static int8_t next_options[S_MAX][V_MAX];
static uint8_t next_len[S_MAX];
static int V, S;
static int row_to_idx[128];

static void init(void)
{
    if (!initialized) {
        // Find all valid rows avoiding 000 and 010 horizontally
        V = 0;
        for (int v = 0; v < 128; ++v) {
            bool ok = true;
            for (int w = 0; w <= 4; ++w) {          // windows of length 3
                int a = (v >> w) & 1;
                int b = (v >> (w + 1)) & 1;
                int c = (v >> (w + 2)) & 1;
                if ((a == 0 && b == 0 && c == 0) ||
                    (a == 0 && b == 1 && c == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                valid_rows[V++] = (uint8_t)v;
            }
        }

        S = V * V;

        // map from 0..127 to index in valid_rows (or -1)
        for (int i = 0; i < 128; ++i) row_to_idx[i] = -1;
        for (int i = 0; i < V; ++i) row_to_idx[valid_rows[i]] = i;

        // precompute admissible next rows for each pair of rows
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                int p = i * V + j;
                int len = 0;
                uint8_t r_i = valid_rows[i];
                uint8_t r_j = valid_rows[j];
                for (int k = 0; k < V; ++k) {
                    uint8_t r_k = valid_rows[k];
                    bool ok = true;
                    for (int col = 0; col < 7 && ok; ++col) {
                        int a = (r_i >> col) & 1;
                        int b = (r_j >> col) & 1;
                        int c = (r_k >> col) & 1;
                        if ((a == 0 && b == 0 && c == 1) ||
                            (a == 0 && b == 1 && c == 0)) {
                            ok = false;
                        }
                    }
                    if (ok) {
                        next_options[p][len++] = (int8_t)k;
                    }
                }
                next_len[p] = (uint8_t)len;
            }
        }

        initialized = true;
    }
}

static seq_t cur[S_MAX];
static seq_t next[S_MAX];

seq_t a(idx_t n)
{
    init();

    if (n == 0) return (seq_t)0;      // undefined offset, but return 0
    if (n == 1) return (seq_t)V;      // first row, any valid row
    if (n == 2) return (seq_t)V * V;  // two rows, no vertical constraints

    // initialize for k = 2 (two rows)
    for (size_t p = 0; p < (size_t)S; ++p) cur[p] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        // zero next
        for (size_t p = 0; p < (size_t)S; ++p) next[p] = (seq_t)0;

        for (size_t p = 0; p < (size_t)S; ++p) {
            seq_t val = cur[p];
            if (val == (seq_t)0) continue;
            uint8_t len = next_len[p];
            int prev2 = p % V;            // second of the pair
            for (int t = 0; t < (int)len; ++t) {
                int idx3 = next_options[p][t];
                size_t q = (size_t)prev2 * V + idx3;
                next[q] += val;
            }
        }
        // copy next to cur for next iteration
        for (size_t p = 0; p < (size_t)S; ++p) cur[p] = next[p];
    }

    // sum over all states to get the total number of arrays
    seq_t ans = (seq_t)0;
    for (size_t p = 0; p < (size_t)S; ++p) ans += cur[p];
    return ans;
}
