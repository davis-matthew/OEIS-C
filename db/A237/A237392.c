/*
 * A237392  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{c=0..6} 1ᵀ A_cⁿ 1, where A_c[s][t] = 1 if the 2×2
 *            block formed by rows encoded by states s and t has value
 *            max+upper_median-lower_median = c. The function computes a(n)
 *            for a (n+1)×2 matrix over {0,1,2,3} whose 2×2 blocks all have a
 *            common constant value
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Adjacency matrices for the 7 possible constants (0..6).  */
static uint8_t adj[7][16][16];

/* Current and next DP vectors for each constant.  */
static seq_t cur[7][16];
static seq_t nxt[7][16];

/* Number of transitions processed (i.e., current n). */
static idx_t current_n = 0;

/* Cache for a(n) values (1‑based).  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Sort four elements (0..3).  */
static void sort4(int a[4])
{
    int t;
    if (a[0] > a[1]) { t = a[0]; a[0] = a[1]; a[1] = t; }
    if (a[2] > a[3]) { t = a[2]; a[2] = a[3]; a[3] = t; }
    if (a[0] > a[2]) { t = a[0]; a[0] = a[2]; a[2] = t; }
    if (a[1] > a[3]) { t = a[1]; a[1] = a[3]; a[3] = t; }
    if (a[1] > a[2]) { t = a[1]; a[1] = a[2]; a[2] = t; }
}

/* Pre‑compute the adjacency matrices.  */
static void init(void)
{
    for (int s = 0; s < 16; ++s)
    {
        int x = s / 4;
        int y = s % 4;
        for (int t = 0; t < 16; ++t)
        {
            int u = t / 4;
            int v = t % 4;
            int a[4] = {x, y, u, v};
            sort4(a);
            int f = a[3] + a[2] - a[1]; /* max + upper_median - lower_median */
            for (int c = 0; c <= 6; ++c)
                adj[c][s][t] = (c == f) ? 1 : 0;
        }
    }
    /* Start with one way to begin at any state (0 transitions). */
    for (int c = 0; c <= 6; ++c)
        for (int s = 0; s < 16; ++s)
            cur[c][s] = (seq_t)1;
}

/* Grow the cache array to hold at least new_cap entries (1‑based). */
static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) exit(EXIT_FAILURE);
    for (idx_t i = 1; i <= cache_max; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) for all n up to the required value. */
static void ensure_up_to(idx_t n)
{
    if (n <= cache_max) return;

    if (cache_cap == 0) {
        grow_cache(32);
        cache[0] = (seq_t)0;            /* unused index 0 */
    }

    static int inited = 0;
    if (!inited) {
        init();
        inited = 1;
    }

    /* Iterate until the cache covers n. */
    while (current_n < n) {
        /* Compute next vector for each constant. */
        for (int c = 0; c <= 6; ++c) {
            for (int t = 0; t < 16; ++t) {
                seq_t sum = (seq_t)0;
                for (int s = 0; s < 16; ++s) {
                    if (adj[c][s][t])
                        sum += cur[c][s];
                }
                nxt[c][t] = sum;
            }
        }

        /* Copy next into current for the next step. */
        for (int c = 0; c <= 6; ++c)
            for (int s = 0; s < 16; ++s)
                cur[c][s] = nxt[c][s];

        /* Total number of valid arrays for this n. */
        seq_t total = (seq_t)0;
        for (int c = 0; c <= 6; ++c)
            for (int s = 0; s < 16; ++s)
                total += cur[c][s];

        ++current_n;

        /* Grow cache if necessary. */
        if ((size_t)(current_n + 1) >= cache_cap)
            grow_cache(cache_cap * 2);

        cache[current_n] = total;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_up_to(n);
    return cache[n];
}
