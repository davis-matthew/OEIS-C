/*
 * A233993  Number of (n+1)X(4+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 18.
 * https://oeis.org/A233993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with entries in 0..6 such that *
 *            every 2×2 subblock has the sum of the absolute values of its six
 *            * edge and diagonal differences equal to 18
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------------------
 *  DP over rows of length 5.  For each possible row we pre‑compute the
 *  list of rows that can follow it.  Then a(n) is obtained by
 *  walking this directed graph n times.  The transition graph is
 *  constructed once on first use; afterwards the DP vector is
 *  multiplied by the adjacency matrix in O(|V|+|E|) per step.
 * ---------------------------------------------------------------------- */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>   /* for abs() */

#define MAX_ROWS 16807   /* 7^5 different rows (5 cells, base‑7 encoding) */

/* transition lists */
static unsigned char next_counts[7][7][7];
static unsigned char next_values[7][7][7][7];

/* temporary storage for building transitions */
static unsigned char left_row[5];
static size_t *start_offsets;   /* row index → start offset in neighbors_arr */
static int *neighbors_arr;      /* flattened adjacency list */
static int *neighbors_ptr;      /* write pointer while building */

/* dynamic programming arrays (two copies swapped each step) */
static seq_t dp1[MAX_ROWS];
static seq_t dp2[MAX_ROWS];
static seq_t *dp_cur, *dp_next;

/* cached values of the sequence */
static seq_t *a_values = NULL;    /* a_values[1..current_max_n] */
static idx_t  current_max_n = 0;
static idx_t  a_values_capacity = 0;

/* ----------------------------------------------------------------------
 *  Dynamic reallocation helpers
 * ---------------------------------------------------------------------- */

static void ensure_a_capacity(idx_t needed)
{
    if (needed <= a_values_capacity)
        return;
    idx_t new_cap = a_values_capacity ? a_values_capacity * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    if (a_values) {
        memcpy(new_arr, a_values, (a_values_capacity + 1) * sizeof(seq_t));
        free(a_values);
    }
    a_values = new_arr;
    a_values_capacity = new_cap;
}

/* ----------------------------------------------------------------------
 *  Recurrence building helpers
 * ---------------------------------------------------------------------- */

static int count_rec(int col, int prev)
{
    if (col == 5)
        return 1;
    int a = left_row[col-1];
    int b = left_row[col];
    int cnt = next_counts[a][b][prev];
    int total = 0;
    for (int i = 0; i < cnt; ++i) {
        int nxt = next_values[a][b][prev][i];
        total += count_rec(col + 1, nxt);
    }
    return total;
}

static void collect_rec(int col, int prev, unsigned char *row_arr)
{
    if (col == 5) {
        int val = 0;
        for (int i = 0; i < 5; ++i)
            val = val * 7 + row_arr[i];
        *neighbors_ptr++ = val;
        return;
    }
    int a = left_row[col-1];
    int b = left_row[col];
    int cnt = next_counts[a][b][prev];
    for (int i = 0; i < cnt; ++i) {
        int nxt = next_values[a][b][prev][i];
        row_arr[col] = nxt;
        collect_rec(col + 1, nxt, row_arr);
    }
}

/* ----------------------------------------------------------------------
 *  Build full transition graph
 * ---------------------------------------------------------------------- */

static void build_graph(void)
{
    /* pre‑compute legal transitions for 2×2 blocks */
    int a, b, c, d;
    memset(next_counts, 0, sizeof(next_counts));
    for (a = 0; a < 7; ++a)
        for (b = 0; b < 7; ++b)
            for (c = 0; c < 7; ++c)
                for (d = 0; d < 7; ++d) {
                    int s = 0;
                    s += abs(a - b);
                    s += abs(c - d);
                    s += abs(a - c);
                    s += abs(b - d);
                    s += abs(a - d);
                    s += abs(b - c);
                    if (s == 18) {
                        next_counts[a][b][c] += 1;
                        next_values[a][b][c][next_counts[a][b][c] - 1] = d;
                    }
                }

    /* Compute degree of each row */
    size_t total_edges = 0;
    static int deg[MAX_ROWS];
    unsigned char row_digits[5];
    for (int row = 0; row < MAX_ROWS; ++row) {
        int tmp = row;
        for (int i = 4; i >= 0; --i) {
            row_digits[i] = tmp % 7;
            tmp /= 7;
        }
        memcpy(left_row, row_digits, 5);
        int sum = 0;
        for (int v0 = 0; v0 < 7; ++v0)
            sum += count_rec(1, v0);
        deg[row] = sum;
        total_edges += sum;
    }

    /* allocate adjacency list */
    neighbors_arr = (int *)malloc(total_edges * sizeof(int));
    start_offsets = (size_t *)malloc((MAX_ROWS + 1) * sizeof(size_t));
    start_offsets[0] = 0;
    for (int i = 0; i < MAX_ROWS; ++i)
        start_offsets[i + 1] = start_offsets[i] + deg[i];

    /* fill adjacency list */
    unsigned char partial[5];
    for (int row = 0; row < MAX_ROWS; ++row) {
        int tmp = row;
        for (int i = 4; i >= 0; --i) {
            row_digits[i] = tmp % 7;
            tmp /= 7;
        }
        memcpy(left_row, row_digits, 5);
        neighbors_ptr = neighbors_arr + start_offsets[row];
        for (int v0 = 0; v0 < 7; ++v0) {
            partial[0] = v0;
            collect_rec(1, v0, partial);
        }
    }
}

/* ----------------------------------------------------------------------
 *  Initialization
 * ---------------------------------------------------------------------- */

static bool initialized = false;

static void init(void)
{
    build_graph();
    dp_cur = dp1;
    dp_next = dp2;
    for (int i = 0; i < MAX_ROWS; ++i)
        dp_cur[i] = (seq_t)1;
    memset(dp_next, 0, MAX_ROWS * sizeof(seq_t));

    a_values_capacity = 64;
    a_values = (seq_t *)malloc((a_values_capacity + 1) * sizeof(seq_t));
    if (!a_values)
        exit(1);
    a_values[0] = (seq_t)0;          /* unused, offset is 1 */
    current_max_n = 0;
    initialized = true;
}

/* ----------------------------------------------------------------------
 *  One DP step (increase the number of rows by 1)
 * ---------------------------------------------------------------------- */

static void step_compute(void)
{
    for (int i = 0; i < MAX_ROWS; ++i) {
        seq_t val = dp_cur[i];
        if (val == (seq_t)0)
            continue;
        size_t st = start_offsets[i];
        size_t ed = start_offsets[i + 1];
        for (size_t j = st; j < ed; ++j) {
            int nbr = neighbors_arr[j];
            dp_next[nbr] += val;
        }
    }

    /* total number of arrays of size (current_max_n+2)×5 */
    seq_t total = (seq_t)0;
    for (int i = 0; i < MAX_ROWS; ++i)
        total += dp_next[i];

    idx_t new_n = current_max_n + 1;
    ensure_a_capacity(new_n);
    a_values[new_n] = total;

    /* swap vectors for next iteration */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;
    ++current_max_n;

    memset(dp_next, 0, MAX_ROWS * sizeof(seq_t));
}

/* ----------------------------------------------------------------------
 *  Public interface
 * ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0)
        return (seq_t)0;          /* offset is 1, a(0) unused */

    while (n > current_max_n)
        step_compute();

    return a_values[n];
}
