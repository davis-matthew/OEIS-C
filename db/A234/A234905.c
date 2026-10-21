/*
 * A234905  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234905: Number of (n+1)×2 arrays with the given constraints.
   Computed by dynamic programming over the directed graph of allowed
   2‑digit pairs.  a(n) equals the number of walks of length n in that
   graph (sum of all entries of the walk matrix A^n). */
#include <stdlib.h>
#include <stdint.h>

/* static data used by the DP */
#define N_STATE 56                 /* number of allowed (x,y) pairs with x!=y */
static uint8_t  nexts[N_STATE][N_STATE]; /* adjacency lists */
static uint8_t  next_cnt[N_STATE];
static int      x_of[N_STATE];
static int      y_of[N_STATE];

static seq_t *cache_arr = NULL;   /* cache of computed a(n) values */
static idx_t  cache_len = 0;      /* largest n that has been computed */

static seq_t *dp_curr = NULL;    /* DP vector for length k */
static seq_t *dp_next = NULL;    /* DP vector for length k+1 */
static int    dp_init  = 0;      /* flag: have the DP buffers been initialized */

static void init_graph(void)
{
    int idx = 0;
    /* enumerate all allowed pairs (x,y) with x!=y */
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (x == y) continue;
            x_of[idx] = x;
            y_of[idx] = y;
            ++idx;
        }
    }
    /* build adjacency lists */
    for (int i = 0; i < N_STATE; ++i) {
        int x1 = x_of[i];
        int y1 = y_of[i];
        next_cnt[i] = 0;
        for (int j = 0; j < N_STATE; ++j) {
            int x2 = x_of[j];
            int y2 = y_of[j];
            if (x1 == x2) continue;      /* no vertical equality */
            if (y1 == y2) continue;      /* no vertical equality */
            int diff = (x1 + y2) - (y1 + x2);
            if (diff == 1 || diff == -1) {
                nexts[i][next_cnt[i]++] = (uint8_t)j;
            }
        }
    }
}

static void init_dp(void)
{
    dp_curr = (seq_t *)calloc(N_STATE, sizeof(seq_t));
    dp_next = (seq_t *)calloc(N_STATE, sizeof(seq_t));
    for (int i = 0; i < N_STATE; ++i) {
        dp_curr[i] = (seq_t)1;          /* paths of length 0 end at every state */
    }
    dp_init = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* ensure global structures are initialized */
    if (!dp_init) {
        init_graph();
        init_dp();
    }

    /* extend cache if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache_arr, new_len * sizeof(seq_t));
        cache_arr = tmp;
        /* compute values from current cache_len+1 up to n */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* compute dp_next from dp_curr */
            for (int j = 0; j < N_STATE; ++j) dp_next[j] = (seq_t)0;
            for (int i = 0; i < N_STATE; ++i) {
                seq_t val = dp_curr[i];
                if (val == (seq_t)0) continue;
                for (int t = 0; t < next_cnt[i]; ++t) {
                    int j = nexts[i][t];
                    dp_next[j] = dp_next[j] + val;
                }
            }
            /* sum over all states to get a(k) */
            seq_t sum = (seq_t)0;
            for (int j = 0; j < N_STATE; ++j) sum = sum + dp_next[j];
            cache_arr[k] = sum;
            /* swap dp_curr and dp_next for next iteration */
            seq_t *tmp = dp_curr;
            dp_curr = dp_next;
            dp_next = tmp;
        }
        cache_len = n;
    }
    return cache_arr[n];
}
