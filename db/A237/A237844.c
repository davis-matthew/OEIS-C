/*
 * A237844  Number of (n+1)X(2+1) 0..2 arrays with the maximum minus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Source code for OEIS sequence A237844
   Counts (n+1)×3 arrays over {0,1,2} where for every 2×2 subblock
   the quantity (maximum − lower_median) is equal across all subblocks.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* helper to compute sorted 4 values and return max - 2nd_smallest */
static inline int diff_of_2x2(const int *a, const int *b, int offset)
{
    int v[4];
    v[0] = a[offset];
    v[1] = a[offset + 1];
    v[2] = b[offset];
    v[3] = b[offset + 1];
    /* simple insertion sort for 4 elements */
    for (int i = 1; i < 4; ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
    return v[3] - v[1];
}

/* precomputed constants */
#define ROWS 27
#define MAX_D 3

/* the main function requested by the harness */
seq_t a(idx_t n)
{
    /* static data, initialized once */
    static bool init_done = false;
    static int digits[ROWS][3];
    static bool edge[MAX_D][ROWS][ROWS];
    static seq_t one = (seq_t)1;

    if (!init_done) {
        /* encode all 3‑digit base‑3 rows */
        for (int r = 0; r < ROWS; ++r) {
            int t = r;
            for (int c = 0; c < 3; ++c) {
                digits[r][2 - c] = t % 3;
                t /= 3;
            }
        }

        /* build transition tables for D=0,1,2 */
        for (int d = 0; d < MAX_D; ++d) {
            for (int i = 0; i < ROWS; ++i)
                for (int j = 0; j < ROWS; ++j) {
                    int d01 = diff_of_2x2(digits[i], digits[j], 0);
                    int d12 = diff_of_2x2(digits[i], digits[j], 1);
                    edge[d][i][j] = (d01 == d12) && (d01 == d);
                }
        }
        init_done = true;
    }

    /* DP for each D */
    seq_t total = (seq_t)0;
    for (int d = 0; d < MAX_D; ++d) {
        seq_t cur[ROWS], next[ROWS];
        for (int i = 0; i < ROWS; ++i) cur[i] = one;

        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < ROWS; ++j) next[j] = (seq_t)0;
            for (int i = 0; i < ROWS; ++i) {
                seq_t val = cur[i];
                if (val == (seq_t)0) continue;
                for (int j = 0; j < ROWS; ++j)
                    if (edge[d][i][j])
                        next[j] += val;
            }
            for (int j = 0; j < ROWS; ++j) cur[j] = next[j];
        }

        seq_t sum = (seq_t)0;
        for (int i = 0; i < ROWS; ++i) sum += cur[i];
        total += sum;
    }
    return total;
}
