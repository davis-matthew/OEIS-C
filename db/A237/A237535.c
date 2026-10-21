/*
 * A237535  Number of (n+1)X(4+1) 0..3 arrays with the upper median minus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A237535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over {0,1,2,3} such that * for
 *            every 2×2 subblock the difference (upper median – lower median)
 *            * is identical for all subblocks
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   block_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds all row states (4^5 = 1024) and for each
 * possible constant difference D ∈ {0,1,2,3} it precomputes transitions
 * from one row to the next that keep the difference equal across the
 * four horizontal 2×2 subblocks.  Then dynamic programming over
 * these transitions counts the number of admissible arrays for any
 * height n+1.  A cache stores results for all n that have been asked
 * so that calls in any order are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/block_diff.h"

#define COLS 5
#define STATE (1 << 10)   /* 4^5 = 1024 states */
#define MAX_D 4          /* differences 0..3 */

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} AdjList;

static AdjList adj[MAX_D][STATE];
static bool built = false;

static void add_edge(int d, int from, int to)
{
    AdjList *L = &adj[d][from];
    if (L->len == L->cap) {
        size_t newcap = L->cap ? (L->cap << 1) : 32;
        L->data = (int *)realloc(L->data, newcap * sizeof(int));
        L->cap  = newcap;
    }
    L->data[L->len++] = to;
}

/* Build adjacency lists for all four possible constant differences */
static void build_adjacency(void)
{
    int rows[STATE][COLS];
    for (int i = 0; i < STATE; ++i) {
        for (int c = 0; c < COLS; ++c)
            rows[i][c] = (i >> (2 * c)) & 3;   /* base‑4 digit extraction */
    }

    for (int d = 0; d < MAX_D; ++d) {
        for (int i = 0; i < STATE; ++i) {
            const int *ri = rows[i];
            for (int j = 0; j < STATE; ++j) {
                const int *rj = rows[j];
                int diff = -1;
                bool ok = true;
                for (int k = 0; k < COLS - 1; ++k) {
                    int val = block_diff(ri[k], ri[k + 1], rj[k], rj[k + 1]);
                    if (diff == -1) diff = val;
                    else if (val != diff) { ok = false; break; }
                }
                if (ok && diff == d) add_edge(d, i, j);
            }
        }
    }
}

/* DP state vectors for each D and parity of row count */
static seq_t vecs[MAX_D][2][STATE];
static int cur_idx[MAX_D] = {0, 0, 0, 0};

/* Cache of previously computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max_n = 0;

/* One‑time initialization */
static void ensure_initialization(void)
{
    if (built) return;
    build_adjacency();
    for (int d = 0; d < MAX_D; ++d)
        for (int i = 0; i < STATE; ++i)
            vecs[d][0][i] = 1;    /* first row can be any state */
    cache = NULL;
    built = true;
}

/* Compute all a(n) up to n (offset 1) */
static void ensure_up_to(idx_t n)
{
    if (n == 0) return;
    ensure_initialization();
    if ((size_t)n <= cache_max_n) return;

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_cap = needed;
    }

    for (idx_t step = cache_max_n + 1; step <= n; ++step) {
        seq_t sum_d[MAX_D] = {0, 0, 0, 0};
        for (int d = 0; d < MAX_D; ++d) {
            int cur = cur_idx[d];
            int nxt = cur ^ 1;
            seq_t *curr_vec = vecs[d][cur];
            seq_t *next_vec = vecs[d][nxt];

            memset(next_vec, 0, sizeof(seq_t) * STATE);

            /* transition */
            for (int i = 0; i < STATE; ++i) {
                seq_t val = curr_vec[i];
                if (val == 0) continue;
                const AdjList *L = &adj[d][i];
                for (size_t e = 0; e < L->len; ++e)
                    next_vec[L->data[e]] += val;
            }

            /* sum all states after this step */
            seq_t s = 0;
            for (int i = 0; i < STATE; ++i) s += next_vec[i];
            sum_d[d] = s;
            cur_idx[d] = nxt;
        }

        seq_t total = 0;
        for (int d = 0; d < MAX_D; ++d) total += sum_d[d];
        cache[step] = total;
    }
    cache_max_n = n;
}

/* Public function required by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_up_to(n);
    return cache[n];
}
