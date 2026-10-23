/*
 * A184212  Number of (n+1)X5 0..3 arrays with every 2X2 subblock sum greater
 *          than 6.
 * https://oeis.org/A184212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with entries 0..3 whose every
 *            2×2 block sum exceeds 6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

/* ----- Global data for the transition graph ----- */
#define STATES (1 << 10)          /* 4^5 = 1024 possible rows */

static unsigned char row_vals[STATES][5];   /* decoded row values */
static int *adj[STATES];                   /* adjacency lists (starts) */
static unsigned short deg[STATES];          /* degrees of each state */
static seq_t *dp_prev = NULL;              /* DP vector after k steps */
static seq_t *dp_next = NULL;              /* temporary DP vector */
static seq_t *a_vals = NULL;               /* cached sequence values a(1)...a(max) */
static size_t a_vals_cap = 0;              /* allocated capacity of a_vals */
static idx_t max_comp = 0;                 /* highest n already computed */

/* ----- Helper to build the transition graph (run once) ----- */
static void build_graph(void)
{
    /* 1. Decode each row state */
    for (int s = 0; s < STATES; ++s) {
        unsigned int x = s;
        for (int c = 0; c < 5; ++c) {
            row_vals[s][c] = (unsigned char)(x & 3);
            x >>= 2;
        }
    }

    /* 2. First pass: compute degrees */
    for (int s = 0; s < STATES; ++s) {
        unsigned short d = 0;
        for (int t = 0; t < STATES; ++t) {
            int ok = 1;
            for (int c = 0; c < 4; ++c) {
                int sum = row_vals[s][c] + row_vals[s][c + 1] +
                          row_vals[t][c] + row_vals[t][c + 1];
                if (sum <= 6) { ok = 0; break; }
            }
            if (ok) ++d;
        }
        deg[s] = d;
    }

    /* 3. Allocate adjacency pool */
    size_t total = 0;
    for (int s = 0; s < STATES; ++s) total += deg[s];
    int *pool = (int*)malloc(total * sizeof(int));
    int *ptr = pool;

    /* 4. Second pass: fill adjacency lists */
    for (int s = 0; s < STATES; ++s) {
        adj[s] = ptr;
        for (int t = 0; t < STATES; ++t) {
            int ok = 1;
            for (int c = 0; c < 4; ++c) {
                int sum = row_vals[s][c] + row_vals[s][c + 1] +
                          row_vals[t][c] + row_vals[t][c + 1];
                if (sum <= 6) { ok = 0; break; }
            }
            if (ok) *ptr++ = t;
        }
    }
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    /* Build the transition graph once */
    static int initialized = 0;
    if (!initialized) {
        build_graph();
        dp_prev = (seq_t*)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t*)malloc(STATES * sizeof(seq_t));
        /* Initialize dp_prev to all ones (step 0) */
        for (int i = 0; i < STATES; ++i) dp_prev[i] = (seq_t)1;
        /* Allocate a_vals with capacity 1 (a(0) unused) */
        a_vals_cap = 1;
        a_vals = (seq_t*)malloc(a_vals_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)0;   /* placeholder */
        max_comp = 0;
        initialized = 1;
    }

    /* Extend a_vals array if necessary */
    if ((size_t)(n + 1) > a_vals_cap) {
        size_t new_cap = a_vals_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *new_arr = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        a_vals = new_arr;
        a_vals_cap = new_cap;
    }

    /* Compute new terms until we reach n */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        /* Zero dp_next */
        memset(dp_next, 0, STATES * sizeof(seq_t));

        /* Transition step */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_prev[s];
            if (val == (seq_t)0) continue;
            int *nbr = adj[s];
            unsigned short d = deg[s];
            for (unsigned short j = 0; j < d; ++j) {
                int t = nbr[j];
                dp_next[t] += val;
            }
        }

        /* Sum all paths of length k+1 (i.e., after k transitions) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < STATES; ++i) sum += dp_next[i];
        a_vals[k] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    max_comp = n;
    return a_vals[n];
}
