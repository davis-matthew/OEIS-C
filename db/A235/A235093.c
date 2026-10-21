/*
 * A235093  Number of (n+1) X (3+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes sequence A235093 via dynamic programming over all legal
 * rows of length 4 (entries 0..5 with no equal neighbours) and the
 * adjacency relation induced by the 2×2 diagonal‑sum difference
 * constraint.  The algorithm builds a graph of rows and, for each
 * input n, counts directed paths of length n in that graph, which
 * equals the required number of (n+1)×4 arrays.  Complexity is
 * O(n · E) with E ≈ 1.5×10^4 for all test cases.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Constants */
#define MAX_STATES 750
#define MAX_CODES 1000

/* ---------- helper list for adjacency lists ---------- */
typedef struct {
    size_t len;
    size_t cap;
    int *data;
} IntList;

static void list_init(IntList *L) { L->len = L->cap = 0; L->data = NULL; }

static void list_push(IntList *L, int value)
{
    if (L->cap == L->len) {
        size_t newcap = (L->cap == 0) ? 16 : L->cap << 1;
        int *tmp = (int *)realloc(L->data, newcap * sizeof(int));
        if (!tmp) abort();
        L->data = tmp;
        L->cap = newcap;
    }
    L->data[L->len++] = value;
}

/* ---------- global tables ---------- */
static int    row_vals[MAX_STATES][4];   /* the four entries of a state */
static int    row_diff[MAX_STATES][3];   /* a[j] - a[j+1] */
static int    row_code[MAX_STATES];      /* code for diff triple */
static IntList edge_list[MAX_STATES];    /* adjacency lists */
static IntList code_lists[MAX_CODES];    /* states grouped by diff code */
static idx_t state_count = 0;            /* actual number of states */

static int diff_code[11] = {
    /* index = d+5 : d=-5..5 */
    0,1,2,3,4,-1,5,6,7,8,9
};

/* ---------- build the transition graph ---------- */
static void build_tables(void)
{
    if (state_count) return;                 /* already built */

    /* initialise code_lists once */
    for (int i = 0; i < MAX_CODES; ++i) list_init(&code_lists[i]);

    /* create all legal rows */
    for (int a0 = 0; a0 <= 5; ++a0)
        for (int a1 = 0; a1 <= 5; ++a1) if (a1 != a0)
            for (int a2 = 0; a2 <= 5; ++a2) if (a2 != a1)
                for (int a3 = 0; a3 <= 5; ++a3) if (a3 != a2)
                {
                    int idx = (int)state_count++;
                    row_vals[idx][0] = a0; row_vals[idx][1] = a1;
                    row_vals[idx][2] = a2; row_vals[idx][3] = a3;
                    row_diff[idx][0] = a0 - a1;
                    row_diff[idx][1] = a1 - a2;
                    row_diff[idx][2] = a2 - a3;

                    int c0 = diff_code[row_diff[idx][0] + 5];
                    int c1 = diff_code[row_diff[idx][1] + 5];
                    int c2 = diff_code[row_diff[idx][2] + 5];
                    row_code[idx] = c0 * 100 + c1 * 10 + c2;

                    list_init(&edge_list[idx]);          /* adjacency list */
                    list_push(&code_lists[row_code[idx]], idx);
                }

    /* build adjacency lists */
    for (int a = 0; a < (int)state_count; ++a)
    {
        int ad[3] = {row_diff[a][0], row_diff[a][1], row_diff[a][2]};
        for (int mask = 0; mask < 8; ++mask)
        {
            int tc[3];
            int ok = 1;
            for (int j = 0; j < 3; ++j)
            {
                int off = ((mask >> j) & 1) ? 4 : -4;
                int d = ad[j] + off;
                if (d == 0 || d < -5 || d > 5) { ok = 0; break; }
                tc[j] = diff_code[d + 5];
            }
            if (!ok) continue;

            int target_code = tc[0] * 100 + tc[1] * 10 + tc[2];
            IntList *L = &code_lists[target_code];
            for (size_t k = 0; k < L->len; ++k)
            {
                int b = L->data[k];
                /* vertical adjacency: a[i] != b[i] */
                if (row_vals[a][0] == row_vals[b][0]) continue;
                if (row_vals[a][1] == row_vals[b][1]) continue;
                if (row_vals[a][2] == row_vals[b][2]) continue;
                if (row_vals[a][3] == row_vals[b][3]) continue;

                /* avoid duplicate edges */
                int dup = 0;
                for (size_t e = 0; e < edge_list[a].len; ++e)
                    if (edge_list[a].data[e] == b) { dup = 1; break; }
                if (!dup) list_push(&edge_list[a], b);
            }
        }
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    build_tables();                        /* first call builds graph */

    static seq_t cur[MAX_STATES];
    static seq_t nxt[MAX_STATES];

    /* initialise: each possible first row contributes 1 array of height 1 */
    for (idx_t i = 0; i < state_count; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step)
    {
        for (idx_t i = 0; i < state_count; ++i) nxt[i] = (seq_t)0;
        for (idx_t i = 0; i < state_count; ++i)
        {
            seq_t val = cur[i];
            if (!val) continue;
            IntList *L = &edge_list[i];
            for (size_t e = 0; e < L->len; ++e)
            {
                int j = L->data[e];
                nxt[j] += val;
            }
        }
        for (idx_t i = 0; i < state_count; ++i) cur[i] = nxt[i];
    }

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < state_count; ++i) result += cur[i];
    return result;
}
