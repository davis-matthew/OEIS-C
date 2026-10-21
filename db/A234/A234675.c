/*
 * A234675  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Empirical recurrence
 *   a(n) = 4*a(n-1)+47*a(n-2)-200*a(n-3)-901*a(n-4)+4204*a(n-5)+8957*a(n-6)
 *         -48440*a(n-7)-47194*a(n-8)+334096*a(n-9)+101948*a(n-10)
 *         -1410080*a(n-11)+170136*a(n-12)+3549696*a(n-13)-1446912*a(n-14)
 *         -4861440*a(n-15)+2954880*a(n-16)+2764800*a(n-17)-2073600*a(n-18)
 *   Initial values for n = 1 … 18 are computed directly by a dynamic‑program
 *   of 3‑column arrays with the 2×2 block constraint.  The recurrence is
 *   used for all n ≥ 19.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static const int NODES = 216;          /* 6³ possible rows of length 3 */
static const int MAX_INIT = 18;        /* number of terms computed by DP */

static int  *adj_start = NULL;        /* size NODES+1 */
static int  *adj_edge  = NULL;        /* flattened adjacency lists */
static int   total_edges = 0;

/* first 18 terms (indices 1…18) computed by DP */
static seq_t init_terms[19] = {0};

/* cache for already computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Build the adjacency graph once.                                     */
/* ------------------------------------------------------------------ */
static void build_graph(void)
{
    int deg[NODES];
    for (int i = 0; i < NODES; ++i) deg[i] = 0;

    /* count outgoing edges for every state */
    for (int s = 0; s < NODES; ++s) {
        int a = s / 36, b = (s / 6) % 6, c = s % 6;
        for (int t = 0; t < NODES; ++t) {
            int d = t / 36, e = (t / 6) % 6, f = t % 6;
            if (abs((a + e) - (b + d)) == 5 &&
                abs((b + f) - (c + e)) == 5)
                ++deg[s];
        }
    }

    total_edges = 0;
    for (int i = 0; i < NODES; ++i) total_edges += deg[i];

    adj_start = (int *)malloc((NODES + 1) * sizeof(int));
    adj_edge  = (int *)malloc(total_edges * sizeof(int));
    adj_start[0] = 0;
    for (int i = 0; i < NODES; ++i)
        adj_start[i + 1] = adj_start[i] + deg[i];

    /* fill adjacency lists */
    int *cur = (int *)malloc(NODES * sizeof(int));
    for (int i = 0; i < NODES; ++i) cur[i] = 0;
    for (int s = 0; s < NODES; ++s) {
        int a = s / 36, b = (s / 6) % 6, c = s % 6;
        for (int t = 0; t < NODES; ++t) {
            int d = t / 36, e = (t / 6) % 6, f = t % 6;
            if (abs((a + e) - (b + d)) == 5 &&
                abs((b + f) - (c + e)) == 5) {
                int pos = adj_start[s] + cur[s]++;
                adj_edge[pos] = t;
            }
        }
    }
    free(cur);
}

/* ------------------------------------------------------------------ */
/* Compute the first 18 terms with a direct DP over rows.             */
/* ------------------------------------------------------------------ */
static void compute_initial_terms(void)
{
    seq_t cur[NODES], nxt[NODES];
    for (int i = 0; i < NODES; ++i) cur[i] = 1;   /* all 1‑row arrays */

    for (int step = 1; step <= MAX_INIT; ++step) {   /* step = number of added rows */
        for (int i = 0; i < NODES; ++i) nxt[i] = 0;
        for (int s = 0; s < NODES; ++s) {
            int start = adj_start[s];
            int end   = adj_start[s + 1];
            for (int p = start; p < end; ++p) {
                int t = adj_edge[p];
                nxt[t] += cur[s];
            }
        }
        seq_t sum = 0;
        for (int i = 0; i < NODES; ++i) sum += nxt[i];
        init_terms[step] = sum;      /* a(step) */
        for (int i = 0; i < NODES; ++i) cur[i] = nxt[i];
    }
}

static bool ready = false;
static void ensure_ready(void)
{
    if (!ready) {
        build_graph();
        compute_initial_terms();
        ready = true;
    }
}

/* ------------------------------------------------------------------ */
/* Recurrence coefficients (index 0 … 17 corresponds to a(n-1)…a(n-18)).  */
/* ------------------------------------------------------------------ */
static const int coeff[18] = {
    4, 47, -200, -901, 4204, 8957, -48440, -47194,
    334096, 101948, -1410080, 170136, 3549696,
    -1446912, -4861440, 2954880, 2764800, -2073600
};

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* the offset is 1; callers use n≥1 */

    ensure_ready();

    /* enlarge cache if necessary */
    if (cache == NULL || (size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill cache up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= MAX_INIT) {
            cache[i] = init_terms[i];
        } else {
            seq_t pos = 0, neg = 0;
            for (int k = 1; k <= 18; ++k) {
                seq_t mult = mul_small(cache[i - k], (unsigned int)abs(coeff[k - 1]));
                if (coeff[k - 1] < 0)
                    neg += mult;
                else
                    pos += mult;
            }
            cache[i] = pos - neg;
        }
    }
    cache_len = n;
    return cache[n];
}
