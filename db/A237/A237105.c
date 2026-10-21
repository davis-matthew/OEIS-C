/*
 * A237105  Number of (n+1)X(5+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237105: Number of (n+1)×6 arrays over {0,1,2} in which
 * the quantity "maximum minus upper median of every 2×2 subblock"
 * is the same for all subblocks.
 *
 * The algorithm precomputes, for each possible difference d∈{0,1,2},
 * the adjacency graph on the 6‑digit base‑3 rows.  An array of
 * n+1 rows satisfies the condition iff all consecutive row pairs are
 * edges of the same graph.  Therefore
 *
 *   a(n) = Σ_{d=0}^{2}  (# of directed paths of length n in the
 *   graph G_d)
 *
 * The path count is obtained by dynamic programming in O(n·E),
 * where E is the total number of allowed edges (≈ 5·10⁵).  The
 * implementation uses helper functions
 *   alloc_seq_array(size_t)   (to allocate seq_t arrays)
 *   free_seq_array(seq_t*)    (to free them).
 *
 * The program is fully static apart from the precomputation of the
 * adjacency lists, which is performed only once.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#define ROWS 729          /* 3^6 possible rows */
#define COLS 6            /* number of columns in the array   */
#define MAX_D 3           /* possible differences 0,1,2      */

/* Pre‑computed row contents (base‑3 representation) */
static unsigned char row_vals[ROWS][COLS];

/* For each difference d, we store adjacency data:
 *   comp_offset[d][r]  = start index of neighbours of row r
 *   comp_count[d][r]   = number of neighbours of row r
 *   comp_buffer[d]     = concatenated neighbour lists
 */
static int comp_offset[MAX_D][ROWS + 1];
static int comp_count[MAX_D][ROWS];
static int *comp_buffer[MAX_D];

/* Build row_vals from 0 … ROWS-1 */
static void
init_rows(void)
{
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        for (int c = COLS - 1; c >= 0; --c) {
            row_vals[r][c] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }
}

/* Check if two consecutive rows r1 (top) and r2 (bottom) produce
 * the same difference d for every 2×2 subblock.
 */
static bool
compatible(int r1, int r2, int d)
{
    for (int col = 0; col < COLS - 1; ++col) {
        int a = row_vals[r1][col];
        int b = row_vals[r1][col + 1];
        int c = row_vals[r2][col];
        int e = row_vals[r2][col + 1];

        /* maximum */
        int maxv = a;
        if (b > maxv) maxv = b;
        if (c > maxv) maxv = c;
        if (e > maxv) maxv = e;

        /* upper median: third smallest */
        int freq[3] = {0, 0, 0};
        ++freq[a];
        ++freq[b];
        ++freq[c];
        ++freq[e];
        int cum = 0, med = 0;
        for (int val = 0; val < 3; ++val) {
            cum += freq[val];
            if (cum >= 3) { med = val; break; }
        }

        if (maxv - med != d)
            return false;
    }
    return true;
}

/* Static pre‑computation of adjacency lists for each difference d */
static void
precompute(void)
{
    static bool initialized = false;
    if (initialized) return;

    init_rows();

    for (int d = 0; d < MAX_D; ++d) {
        /* Count outgoing edges for each row */
        for (int r = 0; r < ROWS; ++r)
            comp_count[d][r] = 0;

        for (int r1 = 0; r1 < ROWS; ++r1)
            for (int r2 = 0; r2 < ROWS; ++r2)
                if (compatible(r1, r2, d))
                    ++comp_count[d][r1];

        /* Compute cumulative offsets and total buffer size */
        comp_offset[d][0] = 0;
        for (int r = 0; r < ROWS; ++r)
            comp_offset[d][r + 1] = comp_offset[d][r] + comp_count[d][r];

        size_t total = (size_t)comp_offset[d][ROWS];
        comp_buffer[d] = (int *)malloc(total * sizeof(int));
        if (!comp_buffer[d]) exit(EXIT_FAILURE);

        /* Fill adjacency lists */
        int *pos = (int *)malloc(ROWS * sizeof(int));
        if (!pos) exit(EXIT_FAILURE);
        for (int r = 0; r < ROWS; ++r) pos[r] = comp_offset[d][r];

        for (int r1 = 0; r1 < ROWS; ++r1) {
            for (int r2 = 0; r2 < ROWS; ++r2) {
                if (compatible(r1, r2, d)) {
                    comp_buffer[d][pos[r1]++] = r2;
                }
            }
        }
        free(pos);
    }
    initialized = true;
}

/* Main sequence function */
seq_t
a(idx_t n)                /* n ≥ 1  (offset is 1) */
{
    precompute();

    /* Allocate DP buffers once */
    seq_t *arr1 = alloc_seq_array(ROWS);
    seq_t *arr2 = alloc_seq_array(ROWS);
    if (!arr1 || !arr2) {
        free_seq_array(arr1);
        free_seq_array(arr2);
        return (seq_t)0;
    }

    seq_t total = 0;

    for (int d = 0; d < MAX_D; ++d) {
        /* Initial state: first row can be any of the ROWS rows */
        for (int r = 0; r < ROWS; ++r)
            arr1[r] = (seq_t)1;          /* cnt_prev */

        seq_t *cnt_prev = arr1;          /* pointer aliases */
        seq_t *cnt_next = arr2;

        for (idx_t step = 1; step <= n; ++step) {
            /* clear next counts */
            for (int r = 0; r < ROWS; ++r)
                cnt_next[r] = (seq_t)0;

            for (int r1 = 0; r1 < ROWS; ++r1) {
                seq_t val = cnt_prev[r1];
                if (!val) continue;

                int offs = comp_offset[d][r1];
                int deg  = comp_count[d][r1];
                for (int k = 0; k < deg; ++k) {
                    int r2 = comp_buffer[d][offs + k];
                    cnt_next[r2] += val;
                }
            }
            /* swap pointers for next step */
            seq_t *tmp = cnt_prev;
            cnt_prev = cnt_next;
            cnt_next = tmp;
        }

        /* Sum all arrays ending with any final row */
        for (int r = 0; r < ROWS; ++r)
            total += cnt_prev[r];
    }

    free_seq_array(arr1);
    free_seq_array(arr2);
    return total;
}
