/*
 * A204726  Number of (n+1)X3 0..4 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3,4} for which all *
 *            2×2 subblock permanents are equal and non‑zero. * * The entry is
 *            computed by dynamic programming on a directed graph whose *
 *            vertices are the 125 possible rows (triplets in base 5). An edge
 *            * (r1→r2) exists with label P if the perma...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t dp_prev[33][125];
static seq_t dp_next[33][125];
static seq_t *cache = NULL;          /* cache[n] = a(n) for n≥1 */
static idx_t cache_size = 0;         /* largest n computed */

struct Edge { unsigned char from; unsigned char to; unsigned char label; };
static struct Edge *edges = NULL;
static size_t edges_cnt = 0;

static void init(void)
{
    if (initialized) return;

    /* Precompute the 125 rows as base‑5 digits. */
    int val[125][3];
    for (int a = 0; a < 5; a++)
        for (int b = 0; b < 5; b++)
            for (int c = 0; c < 5; c++) {
                int r = a * 25 + b * 5 + c;
                val[r][0] = a;
                val[r][1] = b;
                val[r][2] = c;
            }

    /* Build all admissible directed edges labelled by non‑zero common permanent. */
    edges_cnt = 0;
    edges = (struct Edge*)malloc(15625 * sizeof(struct Edge));  /* upper bound */
    for (int r1 = 0; r1 < 125; r1++)
        for (int r2 = 0; r2 < 125; r2++) {
            int p1 = val[r1][0] * val[r2][1] + val[r1][1] * val[r2][0];
            int p2 = val[r1][1] * val[r2][2] + val[r1][2] * val[r2][1];
            if (p1 == p2 && p1 != 0) {
                edges[edges_cnt].from = (unsigned char)r1;
                edges[edges_cnt].to   = (unsigned char)r2;
                edges[edges_cnt].label= (unsigned char)p1;
                edges_cnt++;
            }
        }

    /* Initialise dp_prev to one for all labels and all rows (length‑1 walks). */
    for (int p = 0; p < 33; p++)
        for (int r = 0; r < 125; r++)
            dp_prev[p][r] = 1;
    memset(dp_next, 0, sizeof(dp_next));

    /* Allocate cache and set base size to 0. */
    cache = (seq_t*)calloc(2, sizeof(seq_t));   /* indices 0 and 1 at start */
    cache_size = 0;                              /* a(1) will be computed on demand */
    initialized = true;
}

seq_t a(idx_t n)
{
    init();
    if (n == 0) return 0;          /* not used, offset starts at 1 */

    if (n > cache_size) {
        /* Grow cache to accommodate new values. */
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */
        cache = new_cache;

        /* Dynamic programming: extend from cache_size to n. */
        for (idx_t req = cache_size + 1; req <= n; ++req) {
            /* one step: dp_prev → dp_next */
            for (size_t i = 0; i < edges_cnt; ++i) {
                const struct Edge *e = &edges[i];
                dp_next[e->label][e->to] += dp_prev[e->label][e->from];
            }
            /* swap and clear for next iteration */
            memcpy(dp_prev, dp_next, sizeof(dp_prev));
            memset(dp_next, 0, sizeof(dp_next));

            /* sum over all non‑zero common permanents */
            seq_t total = 0;
            for (int p = 1; p < 33; ++p)
                for (int r = 0; r < 125; ++r)
                    total += dp_prev[p][r];

            cache[req] = total;
        }
        cache_size = n;
    }
    return cache[n];
}
