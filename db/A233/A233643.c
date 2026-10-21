/*
 * A233643  Number of (n+1)X(7+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 2.
 * https://oeis.org/A233643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×8 arrays with entries 0…2
 *               such that every 2×2 sub‑block has the sum of the
 *               squares of its four edge differences equal to 2.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Parameters of the problem */
#define ROWS           6561   /* 3^8 possible rows */
#define COLS           8
#define PAIRS          7      /* number of adjacent column pairs in a row */

/* Global data structures */
static uint8_t digits[ROWS][COLS];   /* base‑3 digits of each row */
static bool   *self_loop = NULL;     /* whether a row has a self‑loop */

/* Edge list between different rows */
struct Edge { uint16_t u, v; };
static struct Edge *edges = NULL;
static size_t edge_cnt = 0, edge_cap = 0;

/* DP arrays */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* Storage of the sequence values */
static seq_t *results = NULL;
static size_t rescap  = 0;      /* size of results array */

/* Number of transitions that have already been processed */
static idx_t processed_n = 0;

/* ------------------------------------------------------------------ */
/* Utility: add a new edge to the list                           */
static void add_edge(uint16_t u, uint16_t v)
{
    if (edge_cnt == edge_cap) {
        edge_cap = edge_cap == 0 ? 1 : edge_cap * 2;
        edges = (struct Edge *)realloc(edges, edge_cap * sizeof(struct Edge));
    }
    edges[edge_cnt].u = u;
    edges[edge_cnt].v = v;
    ++edge_cnt;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute all rows, self‑loop flags, and admissible edges     */
static void init_precalc(void)
{
    /* 1. Pre‑compute base‑3 digits for all rows */
    for (uint32_t r = 0; r < ROWS; ++r) {
        uint32_t v = r;
        for (int j = 0; j < COLS; ++j) {
            digits[r][j] = v % 3;
            v /= 3;
        }
    }

    /* 2. Allocate self‑loop array */
    self_loop = (bool *)malloc(ROWS * sizeof(bool));
    memset(self_loop, 0, ROWS * sizeof(bool));

    /* 3. Compute admissible edges between distinct rows */
    for (uint32_t r1 = 0; r1 < ROWS; ++r1) {
        /* determine if r1 has a self‑loop */
        bool loop = true;
        for (int k = 0; k < PAIRS; ++k) {
            uint8_t a = digits[r1][k];
            uint8_t b = digits[r1][k + 1];
            uint8_t diff = (a > b) ? a - b : b - a;
            if (diff != 1) {          /* need difference exactly 1 */
                loop = false;
                break;
            }
        }
        self_loop[r1] = loop;

        for (uint32_t r2 = r1 + 1; r2 < ROWS; ++r2) {
            bool ok = true;
            for (int k = 0; k < PAIRS; ++k) {
                uint8_t a = digits[r1][k];
                uint8_t b = digits[r1][k + 1];
                uint8_t c = digits[r2][k];
                uint8_t d = digits[r2][k + 1];

                uint8_t diff_t = (a > b) ? a - b : b - a;
                uint8_t diff_b = (c > d) ? c - d : d - c;
                uint8_t diff_l = (a > c) ? a - c : c - a;
                uint8_t diff_r = (b > d) ? b - d : d - b;

                int sum_sq = diff_t * diff_t + diff_b * diff_b
                           + diff_l * diff_l + diff_r * diff_r;
                if (sum_sq != 2) {
                    ok = false;
                    break;
                }
            }
            if (ok) add_edge((uint16_t)r1, (uint16_t)r2);
        }
    }

    /* 4. Allocate DP arrays and initialise */
    dp_cur  = (seq_t *)malloc(ROWS * sizeof(seq_t));
    dp_next = (seq_t *)malloc(ROWS * sizeof(seq_t));
    for (size_t i = 0; i < ROWS; ++i) dp_cur[i] = (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Ensure that a(k) is known for all k ≤ n                           */
static void ensure_computed(idx_t n)
{
    if (n == 0) return;
    if (processed_n >= n) return;

    /* First call – initialise data structures */
    if (!dp_cur) init_precalc();

    /* Grow results array if necessary */
    if ((size_t)n + 1 > rescap) {
        size_t newcap = (size_t)n + 1;
        results = (seq_t *)realloc(results, newcap * sizeof(seq_t));
        /* zero new entries */
        for (size_t i = rescap; i < newcap; ++i) results[i] = (seq_t)0;
        rescap = newcap;
    }

    /* Process steps from processed_n+1 up to n */
    for (idx_t step = processed_n + 1; step <= n; ++step) {
        /* Clear dp_next */
        memset(dp_next, 0, ROWS * sizeof(seq_t));

        /* Self‑loop contributions */
        for (size_t r = 0; r < ROWS; ++r)
            if (self_loop[r]) dp_next[r] += dp_cur[r];

        /* Edge contributions (undirected graph) */
        for (size_t e = 0; e < edge_cnt; ++e) {
            uint16_t u = edges[e].u;
            uint16_t v = edges[e].v;
            dp_next[u] += dp_cur[v];
            dp_next[v] += dp_cur[u];
        }

        /* Swap dp arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Compute a(step) = sum of all entries in dp_cur */
        seq_t sum = (seq_t)0;
        for (size_t r = 0; r < ROWS; ++r) sum += dp_cur[r];
        results[step] = sum;
    }

    processed_n = n;
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_computed(n);
    return results[n];
}
