/*
 * A204784  Number of (n+1)X2 0..7 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{K=1}^{98} Σ_{u,v} (M_K)^n[u][v]
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     M_K[u][v] = 1  iff  (x_u,y_u)·(x_v,y_v) = a*d + b*c = K,  K>0 */
/*     Where (x_u,y_u) and (x_v,y_v) ∈ {0,…,7}² encode the 64 rows of a 2‑column array. */
/* This function computes the OEIS sequence A204784 by dynamic programming. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper to compute the 64‑bit mask of a 2‑dimensional index */
static inline unsigned int idx(int a, int b) { return (a << 3) | b; }

/* adjacency: neigh[K][u] holds the list of v for which M_K[u][v] = 1 */
#define MAX_K 98
#define DIM 64
#define MAX_DEG 64  /* maximum possible degree when a or b is 0 */

static bool tables_built = false;
static unsigned int deg[MAX_K + 1][DIM];
static uint8_t neigh[MAX_K + 1][DIM][MAX_DEG];

/* Build adjacency lists for all permanent values K = 1..98 */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d) {
                    unsigned int K = a * d + b * c;
                    if (K == 0 || K > MAX_K) continue;
                    unsigned int u = idx(a, b);
                    unsigned int v = idx(c, d);
                    unsigned int dgt = deg[K][u];
                    neigh[K][u][dgt] = (uint8_t)v;
                    deg[K][u] = dgt + 1;
                }
}

/* dynamic programming tables for each K and current row length */
static seq_t dp_cur[MAX_K + 1][DIM];
static seq_t dp_next[MAX_K + 1][DIM];

/* cache of sequence values a(1) … a(max_computed) */
static seq_t *a_cache = NULL;       /* 0‑based: a_cache[0] unused */
static idx_t  precomputed_n = 0;    /* maximum n that has been computed */

/* extend the cache and DP tables up to the desired n (1‑based) */
static void extend_to(idx_t n)
{
    if (n <= precomputed_n) return;

    build_tables();

    /* allocate or grow cache */
    size_t new_size = (size_t)(n + 1);
    a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (precomputed_n + 1 < new_size)
        for (size_t i = precomputed_n + 1; i < new_size; ++i)
            a_cache[i] = (seq_t)0;

    /* initialise DP tables for the first step if this is the very first call */
    if (precomputed_n == 0) {
        for (int K = 1; K <= MAX_K; ++K)
            for (int u = 0; u < DIM; ++u)
                dp_cur[K][u] = (seq_t)1;
    }

    /* incremental dynamic programming */
    for (idx_t step = precomputed_n + 1; step <= n; ++step) {
        seq_t sum_n = (seq_t)0;
        for (int K = 1; K <= MAX_K; ++K) {
            /* reset next vector */
            for (int v = 0; v < DIM; ++v)
                dp_next[K][v] = (seq_t)0;

            /* propagate one transition */
            for (int u = 0; u < DIM; ++u) {
                seq_t val = dp_cur[K][u];
                if (!val) continue;
                unsigned int dgt = deg[K][u];
                for (unsigned int ii = 0; ii < dgt; ++ii) {
                    unsigned int v = neigh[K][u][ii];
                    dp_next[K][v] += val;
                }
            }

            /* swap next into current */
            for (int v = 0; v < DIM; ++v)
                dp_cur[K][v] = dp_next[K][v];

            /* add contribution of this K to a(step) */
            for (int v = 0; v < DIM; ++v)
                sum_n += dp_cur[K][v];
        }
        a_cache[step] = sum_n;
    }
    precomputed_n = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 0 not used */
    extend_to(n);
    return a_cache[n];
}
