/*
 * A233647  Number of (n+1)X(2+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 14.
 * https://oeis.org/A233647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233647 – Number of (n+1)×3 0..5 arrays with every 2×2 subblock
 * having sum of squares of the edge differences equal to 14.
 *
 * The implementation builds a directed graph on all 3‑digit base‑6
 * states (216 nodes).  An edge from state i to state j exists iff the
 * 2×2 block formed by rows represented by i and j satisfies the
 * required condition.  A sequence of length n+1 rows corresponds to a
 * path of length n in this graph, so a(n) is the number of length‑n
 * walks beginning at any state.  We perform a breadth‑first dynamic
 * programming that accumulates these walk counts; the result for each
 * n is cached to enable out‑of‑order calls.
 *
 * For n=1..18 the function returns
 *   772, 5232, 36784, 272564, 2012472, 15071680, 112180004,
 *   840367588, 6267926076, 46922305260, 350281544876,
 *   2620797101560, 19574181255752, 146400460065172,
 *   1093757260135120, 8178623368338420, 61113653767807332,
 *   456914521395216488
 *
 * This algorithm runs in O(n * 216 * 216) time and uses only a few
 * kilobytes of memory; it meets the 2‑second per call requirement for
 * n up to about 10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool          init_done          = false;
static uint8_t       adj[216][216];
static uint16_t      nb[216][216];
static uint16_t      deg[216];
static seq_t       *dp_cur             = NULL;   /* DP for step k‑1 */
static seq_t       *dp_next            = NULL;   /* DP for step k   */
static seq_t       *cache              = NULL;   /* a[1..cache_cap] */
static size_t        cache_cap          = 0;      /* largest n stored */

/* initialise graph and DP buffers once */
static void init(void)
{
    /* build adjacency matrix */
    for (int i = 0; i < 216; ++i) {
        int a0 = i / 36;
        int a1 = (i / 6) % 6;
        int a2 = i % 6;
        for (int j = 0; j < 216; ++j) {
            int b0 = j / 36;
            int b1 = (j / 6) % 6;
            int b2 = j % 6;
            int d1 = a0 - a1;
            int d2 = b0 - b1;
            int d3 = a0 - b0;
            int d4 = a1 - b1;
            int s1 = d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4;
            d1 = a1 - a2;
            d2 = b1 - b2;
            d3 = a1 - b1;
            d4 = a2 - b2;
            int s2 = d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4;
            adj[i][j] = (s1 == 14 && s2 == 14) ? 1 : 0;
        }
    }

    /* build adjacency lists */
    for (int i = 0; i < 216; ++i) {
        uint16_t k = 0;
        for (int j = 0; j < 216; ++j)
            if (adj[i][j]) nb[i][k++] = (uint16_t)j;
        deg[i] = k;
    }

    /* allocate DP arrays */
    dp_cur  = (seq_t *)malloc(sizeof(seq_t) * 216);
    dp_next = (seq_t *)malloc(sizeof(seq_t) * 216);
    for (int i = 0; i < 216; ++i) dp_cur[i] = (seq_t)1; /* one row */

    init_done = true;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* outside the defined offset */

    if (!init_done) init();

    /* enlarge and fill cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        for (size_t i = cache_cap + 1; i < newsize; ++i)
            cache[i] = (seq_t)0;

        /* compute values from cache_cap+1 up to n */
        for (size_t step = cache_cap + 1; step <= (size_t)n; ++step) {
            /* reset dp_next */
            for (int j = 0; j < 216; ++j) dp_next[j] = (seq_t)0;

            /* transition */
            for (int i = 0; i < 216; ++i) {
                seq_t v = dp_cur[i];
                if (!v) continue;
                for (uint16_t t = 0; t < deg[i]; ++t) {
                    int j = nb[i][t];
                    dp_next[j] += v;
                }
            }

            /* total number of arrays of size step+1 rows */
            seq_t total = (seq_t)0;
            for (int j = 0; j < 216; ++j) total += dp_next[j];
            cache[step] = total;

            /* swap current and next for next iteration */
            seq_t *hold = dp_cur;
            dp_cur = dp_next;
            dp_next = hold;
        }
        cache_cap = (size_t)n;
    }

    return cache[n];
}
