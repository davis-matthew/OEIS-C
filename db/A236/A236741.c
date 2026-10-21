/*
 * A236741  Number of (n+1)X(3+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A236741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S=0}^{4} count(n,S), where count(n,S) is the number
 *            of (n+1)×4 arrays over {0,1,2} such that in every 2×2 subblock
 *            the two middle values sum to S. The values are computed by
 *            dynamic programming over the rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define ROWS 81       /* 3⁴ possible rows */
#define SCNT 5        /* possible sums of the two middle values (0..4) */

/* Precomputed digits of each row in base‑3: row[i][c] is the value at column c */
static unsigned char row_digits[ROWS][4];
/* For each sum S and source row i: number of allowed destination rows and the list */
static unsigned char neighbor_cnt[SCNT][ROWS];
static unsigned char neighbor_list[SCNT][ROWS][ROWS];
static bool adjacency_built = false;

/* Build adjacency lists once */
static void build_adjacency(void)
{
    if (adjacency_built) return;
    adjacency_built = true;

    /* Convert each row number to its 4 base‑3 digits */
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int p = 0; p < 4; ++p) {
            row_digits[r][p] = (unsigned char)(v % 3);
            v /= 3;
        }
    }

    /* For each sum S, list all transitions between rows that keep
       all 2×2 subblocks with middle‑sum equal to S. */
    for (int i = 0; i < ROWS; ++i) {
        for (int s = 0; s < SCNT; ++s) neighbor_cnt[s][i] = 0;
        for (int j = 0; j < ROWS; ++j) {
            for (int s = 0; s < SCNT; ++s) {
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    unsigned char a = row_digits[i][c];
                    unsigned char b = row_digits[i][c + 1];
                    unsigned char c0 = row_digits[j][c];
                    unsigned char d = row_digits[j][c + 1];
                    unsigned char tot = a + b + c0 + d;
                    unsigned char minv = a;
                    if (b < minv) minv = b;
                    if (c0 < minv) minv = c0;
                    if (d < minv) minv = d;
                    unsigned char maxv = a;
                    if (b > maxv) maxv = b;
                    if (c0 > maxv) maxv = c0;
                    if (d > maxv) maxv = d;
                    unsigned char summid = tot - minv - maxv;
                    if (summid != (unsigned char)s) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    unsigned char *list = neighbor_list[s][i];
                    list[neighbor_cnt[s][i]++] = (unsigned char)j;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_adjacency();

    static seq_t dp_prev[ROWS];
    static seq_t dp_next[ROWS];
    seq_t total = 0;

    for (int s = 0; s < SCNT; ++s) {
        /* start with all rows possible for the first row */
        for (int i = 0; i < ROWS; ++i) dp_prev[i] = 1;

        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < ROWS; ++j) dp_next[j] = 0;
            for (int i = 0; i < ROWS; ++i) {
                seq_t val = dp_prev[i];
                if (!val) continue;
                unsigned char cnt = neighbor_cnt[s][i];
                unsigned char *list = neighbor_list[s][i];
                for (unsigned char k = 0; k < cnt; ++k)
                    dp_next[ list[k] ] += val;
            }
            for (int j = 0; j < ROWS; ++j) dp_prev[j] = dp_next[j];
        }

        seq_t sum = 0;
        for (int j = 0; j < ROWS; ++j) sum += dp_prev[j];
        total += sum;
    }

    return total;
}
