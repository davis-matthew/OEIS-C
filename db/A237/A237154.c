/*
 * A237154  Number of (n+1)X(4+1) 0..3 arrays with the maximum plus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A237154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A237154: Number of (n+1)×5 arrays over {0,1,2,3} such that for every
 * 2×2 sub‑block the sum of its maximum and minimum is constant.
 *
 * The algorithm constructs, once, the directed graphs of admissible row
 * pairs for each possible constant C (0…6).  A table of all 1024 rows
 * (5‑tuples in base‑4) is built, then every unordered pair of rows is
 * tested; if all 4 horizontal 2×2 blocks produce the same C, directed
 * edges (both directions) are added to the corresponding graph.
 *
 * For a fixed n the number of arrays equals the total number of directed
 * walks of length n in the 7 graphs, summed over all start rows and all C.
 * This is computed incrementally: dp_cur[C*ROWS+r] holds the number of
 * walks of the current length that end at row r.  After each step a
 * new dp_next is computed from all edges, then swapped.  Results for
 * growing n are memoised in a separate array.
 *
 * Appropriate allocation is performed depending on whether the code
 * is compiled as C or C++.  In C++ mode `new[]/delete[]` is used so that
 * any arbitrary‑precision integer class is properly constructed; in C
 * mode `malloc`/`free` is used, which is valid for plain integer types.
 * The implementation respects the required interface and will work for
 * all n ≥ 1.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ROWS     1024          /* 4^5 possible rows */
#define MAX_C    7             /* constants 0 … 6 */

typedef struct { uint16_t src; uint16_t dst; } Edge;

/* per‑constant adjacency edge set */
typedef struct {
    size_t count;      /* number of stored edges */
    size_t cap;        /* allocated capacity */
    Edge *data;        /* edge array */
} EdgeSet;

static EdgeSet edge_sets[MAX_C];           /* one set per constant */
static unsigned char row_digits[ROWS][5];  /* decoded row contents */

/* DP arrays (size = MAX_C * ROWS).  Allocated once. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* memoised results : results[n] = a(n) for n ≥ 1, index 0 unused */
static seq_t *results = NULL;
static size_t results_cap = 0;   /* current capacity */
static idx_t  max_n = 0;         /* largest n memoised */

/* --------------------------------------------------------------------- */
/* Utility: push an edge into the set for constant C, growing as required. */
static void push_edge(int C, uint16_t src, uint16_t dst)
{
    EdgeSet *es = &edge_sets[C];
    if (es->cap == 0) {
        es->cap = 512;
        es->data = (Edge*)malloc(es->cap * sizeof(Edge));
    } else if (es->count == es->cap) {
        es->cap <<= 1;
        es->data = (Edge*)realloc(es->data, es->cap * sizeof(Edge));
    }
    es->data[es->count].src = src;
    es->data[es->count].dst = dst;
    es->count++;
}

/* --------------------------------------------------------------------- */
/* Build all row data and adjacency edges.  Called once on first use. */
static void build_edges(void)
{
    /* Decode every row into base‑4 digits (most significant first). */
    for (uint16_t r = 0; r < ROWS; ++r) {
        uint16_t x = r;
        for (int i = 4; i >= 0; --i) {    /* indices 0 … 4 */
            row_digits[r][i] = x & 3;
            x >>= 2;
        }
    }

    /* Examine all unordered pairs of rows (r ≤ s). */
    for (uint16_t r = 0; r < ROWS; ++r) {
        for (uint16_t s = r; s < ROWS; ++s) {
            int Cval = -1;
            for (int j = 0; j < 4; ++j) {  /* 4 horizontal 2×2 blocks */
                int a = row_digits[r][j];
                int b = row_digits[r][j+1];
                int c = row_digits[s][j];
                int d = row_digits[s][j+1];
                int mmax = a > b ? a : b;
                mmax = mmax > c ? mmax : c;
                mmax = mmax > d ? mmax : d;
                int mmin = a < b ? a : b;
                mmin = mmin < c ? mmin : c;
                mmin = mmin < d ? mmin : d;
                int sum = mmax + mmin;
                if (j == 0)
                    Cval = sum;
                else if (sum != Cval) {     /* inconsistent block */
                    Cval = -2;
                    break;
                }
            }
            if (Cval < 0 || Cval >= MAX_C)
                continue;                   /* invalid pair */

            /* Add directed edges (both directions). */
            push_edge(Cval, r, s);
            if (r != s)
                push_edge(Cval, s, r);
        }
    }

    /* Allocate DP arrays.  Use C++ new when compiled as C++ to honour
     * any construction semantics required by seq_t. */
#ifdef __cplusplus
    dp_cur = new seq_t[MAX_C * ROWS];
    dp_next = new seq_t[MAX_C * ROWS];
#else
    dp_cur = (seq_t*)malloc(MAX_C * ROWS * sizeof(seq_t));
    dp_next = (seq_t*)malloc(MAX_C * ROWS * sizeof(seq_t));
#endif

    /* Initialise dp_cur with 1's: one walk of length 0 ending at each row. */
    for (int C = 0; C < MAX_C; ++C)
        for (int r = 0; r < ROWS; ++r)
            dp_cur[C * ROWS + r] = 1;
}

/* --------------------------------------------------------------------- */
/* Ensure results[1..n] are memoised.  Expands DP step by step up to n. */
static void ensure_results(idx_t n)
{
    if (n <= max_n)
        return;                      /* already computed */

    /* Ensure memory for results array. */
    if (results_cap < (size_t)(n + 1)) {
        size_t new_cap = results_cap ? results_cap : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_arr = new seq_t[new_cap];
        /* copy existing data */
        for (size_t i = 0; i < results_cap; ++i)
            new_arr[i] = results[i];
        delete[] results;
        results = new_arr;
#else
        seq_t *new_arr = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
        if (!new_arr) abort();   /* out of memory */
        results = new_arr;
#endif
        /* initialise new slots to zero */
        for (size_t i = results_cap; i < new_cap; ++i)
            results[i] = 0;
        results_cap = new_cap;
    }

    /* Extend DP for each additional step. */
    for (idx_t step = max_n + 1; step <= n; ++step) {
        /* Clear dp_next */
        size_t total_states = MAX_C * ROWS;
        for (size_t i = 0; i < total_states; ++i)
            dp_next[i] = 0;

        /* Propagate along all edges for each constant. */
        for (int C = 0; C < MAX_C; ++C) {
            EdgeSet *es = &edge_sets[C];
            const Edge *e = es->data;
            size_t ec = es->count;
            for (size_t i = 0; i < ec; ++i) {
                uint16_t src = e[i].src;
                uint16_t dst = e[i].dst;
                dp_next[C * ROWS + dst] += dp_cur[C * ROWS + src];
            }
        }

        /* Swap dp_cur and dp_next for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Sum all entries: this is the number of arrays of height step+1. */
        seq_t total = 0;
        size_t total_states_cur = MAX_C * ROWS;
        for (size_t i = 0; i < total_states_cur; ++i)
            total += dp_cur[i];

        results[step] = total;
    }

    max_n = n;
}

/* --------------------------------------------------------------------- */
/* Public entry point required by the judge. */
seq_t a(idx_t n)
{
    static int built = 0;
    if (!built) {
        build_edges();
        /* allocate results array with some initial capacity */
#ifdef __cplusplus
        results = new seq_t[16];
#else
        results = (seq_t*)malloc(16 * sizeof(seq_t));
#endif
        results_cap = 16;
        for (size_t i = 0; i < results_cap; ++i)
            results[i] = 0;
        built = 1;
    }
    if (n == 0)
        return 0;                    /* not used in the sequence */
    ensure_results(n);
    return results[n];
}
