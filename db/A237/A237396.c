/*
 * A237396  Number of (n+1) X (5+1) 0..3 arrays with the maximum plus the
 *          upper median minus the lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A237396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The number of arrays is the sum over s = 0..6 of the number of *
 *            sequences of n+1 rows (each row is a 6‑tuple of digits 0..3)
 *            such that * for every adjacent pair of rows the value * max +
 *            upper‑median – lower‑median * computed from the five 2×2 blocks
 *            that the pair defines is equal to s. * The co...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 4096            /* 4^6 possible rows */

/* Row values in base‑4 representation */
static unsigned char row_vals[ROWS][6];

/* Dynamic adjacency lists per value S (0..6) */
typedef struct {
    idx_t *next;      /* array of destination rows */
    size_t size;      /* number of entries */
    size_t cap;       /* current capacity */
} EdgeList;

static EdgeList adj[7][ROWS];

/* Append a destination to an adjacency list */
static void add_edge(idx_t s, idx_t from, idx_t to)
{
    EdgeList *el = (EdgeList *)&adj[(size_t)s][(size_t)from];
    if (el->size == el->cap) {
        size_t newcap = el->cap ? (el->cap << 1) : 4;
        idx_t *tmp = (idx_t *)realloc(el->next, newcap * sizeof(idx_t));
        if (!tmp) exit(1);
        el->next = tmp;
        el->cap = newcap;
    }
    el->next[el->size++] = to;
}

/* Zero a seq_t array of length N */
static void zero_seq(seq_t *arr, idx_t N)
{
    for (idx_t i = 0; i < N; ++i)
        arr[i] = (seq_t)0;
}

/* Precompute rows and adjacency lists once */
static void init(void)
{
    /* Construct rows as base‑4 strings */
    for (idx_t r = 0; r < ROWS; ++r) {
        idx_t v = r;
        for (int k = 5; k >= 0; --k) {
            row_vals[r][k] = (unsigned char)(v & 3U);
            v >>= 2;
        }
    }

    /* Initialise adjacency lists */
    for (idx_t s = 0; s < 7; ++s) {
        for (idx_t i = 0; i < ROWS; ++i) {
            adj[s][i].next = NULL;
            adj[s][i].size = 0;
            adj[s][i].cap  = 0;
        }
    }

    /* Find all valid row pairs */
    for (idx_t i = 0; i < ROWS; ++i) {
        for (idx_t j = 0; j < ROWS; ++j) {
            bool ok = true;
            int first_val = 0;
            for (int k = 0; k < 5; ++k) {
                int a0 = row_vals[i][k];
                int a1 = row_vals[i][k + 1];
                int b0 = row_vals[j][k];
                int b1 = row_vals[j][k + 1];

                int freq[4] = {0};
                ++freq[a0];
                ++freq[a1];
                ++freq[b0];
                ++freq[b1];

                /* lower median (second smallest) */
                int cum = 0, lower = 0;
                for (int t = 0; t < 4; ++t) {
                    cum += freq[t];
                    if (cum >= 2) { lower = t; break; }
                }

                /* upper median (second largest) */
                cum = 0;
                int upper = 3;
                for (int t = 3; t >= 0; --t) {
                    cum += freq[t];
                    if (cum >= 2) { upper = t; break; }
                }

                /* min and max present */
                int minv = 0;
                while (minv < 4 && freq[minv] == 0) ++minv;
                int maxv = 3;
                while (maxv >= 0 && freq[maxv] == 0) --maxv;

                int val = maxv + upper - lower;
                if (k == 0) first_val = val;
                else if (val != first_val) { ok = false; break; }
            }
            if (ok)
                add_edge((idx_t)first_val, i, j);
        }
    }
}

seq_t a(idx_t n)
{
    static bool inited = false;
    if (!inited) { init(); inited = true; }

    if (n == 0) return (seq_t)0;     /* a(0) is undefined for offset 1 */

    /* Allocate DP arrays */
    seq_t *dp = (seq_t *)calloc(ROWS, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(ROWS, sizeof(seq_t));
    if (!dp || !dp_next) exit(1);

    seq_t total = (seq_t)0;

    for (idx_t s = 0; s < 7; ++s) {
        /* Initial DP: one path starts at each row */
        for (idx_t i = 0; i < ROWS; ++i)
            dp[i] = (seq_t)1;
        zero_seq(dp_next, ROWS);

        /* Perform n transitions */
        for (idx_t step = 0; step < n; ++step) {
            for (idx_t i = 0; i < ROWS; ++i) {
                seq_t cur = dp[i];
                if (cur == (seq_t)0) continue;
                EdgeList *el = &adj[(size_t)s][(size_t)i];
                for (size_t e = 0; e < el->size; ++e) {
                    idx_t j = el->next[e];
                    dp_next[j] += cur;
                }
            }
            /* swap dp and dp_next, clear dp_next */
            seq_t *tmp = dp;
            dp = dp_next;
            dp_next = tmp;
            zero_seq(dp_next, ROWS);
        }

        /* Sum all paths for this s */
        for (idx_t i = 0; i < ROWS; ++i)
            total += dp[i];
    }

    free(dp);
    free(dp_next);
    return total;
}
