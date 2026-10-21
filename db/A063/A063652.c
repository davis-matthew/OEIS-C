/*
 * A063652  Number of ways to tile an 8 X n rectangle with 1 X 1 and 2 X 2
 *          tiles.
 * https://oeis.org/A063652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of tilings of an 8×n rectangle with 1×1 and
 *            2×2 squares. Computed by a transfer‑matrix DP with 256 column
 *            profiles
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t T[256][256] = {{0}};          /* transition counts between profiles */
static struct { uint8_t next; seq_t ways; } edges[256][256] = {{0}};
static int edge_count[256] = {0};

static void dfs_fill(uint8_t base, uint8_t mask, uint8_t next)
{
    int r;
    for (r = 0; r < 8; ++r)
        if (!(mask & (1 << r))) break;

    if (r == 8) {                /* profile completely filled */
        T[base][next] = T[base][next] + ((seq_t)1);
        return;
    }

    /* Place a 1×1 tile */
    dfs_fill(base, mask | (1 << r), next);

    /* Place a 2×2 tile if possible */
    if (r < 7 && !(mask & (1 << (r + 1)))) {
        uint8_t new_next = next | (1 << r) | (1 << (r + 1));
        dfs_fill(base, mask | (1 << r) | (1 << (r + 1)), new_next);
    }
}

static void build_transitions(void)
{
    static bool done = false;
    if (done) return;
    for (int i = 0; i < 256; ++i) {
        dfs_fill((uint8_t)i, (uint8_t)i, 0);
    }

    /* Build edge lists from the dense matrix */
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            if (T[i][j] != ((seq_t)0)) {
                edges[i][edge_count[i]].next = (uint8_t)j;
                edges[i][edge_count[i]].ways = T[i][j];
                edge_count[i]++;
            }
        }
    }
    done = true;
}

/* DP state for the current column */
static seq_t dp[256], nextdp[256];
/* Cache of sequence values up to the largest n computed so far */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached terms   (cache_len == 0 -> nothing cached) */
static size_t max_computed = 0;/* largest n for which cache[ n ] is valid */

seq_t a(idx_t n)
{
    if (cache == NULL) {           /* first call: initialise */
        build_transitions();
        dp[0] = ((seq_t)1);
        for (int i = 1; i < 256; ++i) dp[i] = ((seq_t)0);
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = dp[0];
        max_computed = 0;
    }

    if (n < cache_len)            /* already computed */
        return cache[n];

    /* Extend computation up to n */
    for (size_t step = max_computed + 1; step <= n; ++step) {
        memset(nextdp, 0, sizeof(nextdp));
        for (int i = 0; i < 256; ++i) {
            if (dp[i] == ((seq_t)0)) continue;
            int ec = edge_count[i];
            for (int e = 0; e < ec; ++e) {
                uint8_t j = edges[i][e].next;
                seq_t ways = edges[i][e].ways;
                nextdp[j] = nextdp[j] + dp[i] * ways;
            }
        }
        /* Ensure cache large enough */
        if (step >= cache_len) {
            seq_t *tmp = (seq_t*)realloc(cache, (step + 1) * sizeof(seq_t));
            if (!tmp) { free(cache); exit(EXIT_FAILURE); }
            cache = tmp;
            cache_len = step + 1;
        }
        cache[step] = nextdp[0];   /* a(step) is the number of ways with empty profile */
        memcpy(dp, nextdp, sizeof(dp));
    }

    max_computed = n;
    return cache[n];
}
