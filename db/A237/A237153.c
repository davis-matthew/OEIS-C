/*
 * A237153  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A237153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..3 such that *
 *            for every 2×2 subblock the sum of its maximum and minimum equals
 *            a * constant independent of the subblock. This implementation
 *            counts all * arrays directly using dynamic programming over row
 *            states
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool builtin = false;
static unsigned char row_val[256][4];
static int next_arr[7][256][256];
static int next_len[7][256];

/* Build transition tables for all possible sums S (0..6). */
static void build_tables(void)
{
    if (builtin) return;
    builtin = true;

    /* Precompute the four cell values for each 4‑tuple state. */
    for (int s = 0; s < 256; ++s) {
        row_val[s][0] = (s >> 0) & 3;
        row_val[s][1] = (s >> 2) & 3;
        row_val[s][2] = (s >> 4) & 3;
        row_val[s][3] = (s >> 6) & 3;
    }

    /* For each possible constant S, determine admissible transitions. */
    for (int S = 0; S <= 6; ++S) {
        for (int i = 0; i < 256; ++i) {
            next_len[S][i] = 0;
            for (int j = 0; j < 256; ++j) {
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col) {
                    int a = row_val[i][col];
                    int b = row_val[i][col + 1];
                    int c = row_val[j][col];
                    int d = row_val[j][col + 1];
                    int mn = a, mx = a;
                    if (b < mn) mn = b; if (b > mx) mx = b;
                    if (c < mn) mn = c; if (c > mx) mx = c;
                    if (d < mn) mn = d; if (d > mx) mx = d;
                    if (mn + mx != S) ok = false;
                }
                if (ok) {
                    int k = next_len[S][i];
                    next_arr[S][i][k] = j;
                    ++next_len[S][i];
                }
            }
        }
    }
}

/* Count sequences of length n+1 (= rows) for a fixed constant S. */
static seq_t count_for_sum(idx_t n, int S)
{
    /* dp_cur[i] = number of ways to reach row state i after current step. */
    static seq_t dp_cur[256];
    static seq_t dp_next[256];
    seq_t zero = 0;

    for (int i = 0; i < 256; ++i) dp_cur[i] = 1;   /* first row arbitrary */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        for (int i = 0; i < 256; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            int lim = next_len[S][i];
            const int *arr = next_arr[S][i];
            for (int k = 0; k < lim; ++k) {
                int j = arr[k];
                dp_next[j] += val;
            }
        }
        /* swap current and next */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all possible ending states */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += dp_cur[i];
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; return 0 for n=0 if called */
    build_tables();

    seq_t result = 0;
    for (int S = 0; S <= 6; ++S)
        result += count_for_sum(n, S);
    return result;
}
