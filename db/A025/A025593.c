/*
 * A025593  Number of n-move bishop paths on 8x8 board from given corner to
 *          same corner.
 * https://oeis.org/A025593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   computed via dynamic programming on the 64 squares of an 8×8
 *            board counting the number of bishop paths of length n starting
 *            and ending at the corner square (0,0). The function returns
 *            a(idx_t n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static state ------------------------------------------------------------ */
static bool       initialized = false;
static size_t     max_cached  = 0;           /* largest n cached so far     */
static size_t     cache_cap   = 0;           /* allocated size of cache     */
static seq_t     *cache       = NULL;        /* a(n) values                  */
static seq_t      cur[64], nxt[64];          /* counts for current and next step */
static int        nbr[64][14];              /* adjacency lists of bishop moves */
static int        nbr_len[64];              /* number of neighbours per square */

/* generate adjacency lists for all 64 squares -------------------------------- */
static void generate_adjacency(void)
{
    for (int i = 0; i < 64; ++i) {
        int r1 = i / 8;
        int c1 = i % 8;
        int len = 0;
        for (int j = 0; j < 64; ++j) {
            if (j == i) continue;
            int r2 = j / 8;
            int c2 = j % 8;
            int dr = r1 - r2;  if (dr < 0) dr = -dr;
            int dc = c1 - c2;  if (dc < 0) dc = -dc;
            if (dr == dc) {
                nbr[i][len] = j;
                ++len;
            }
        }
        nbr_len[i] = len;
        for (; len < 14; ++len) nbr[i][len] = -1;    /* padding, unused */
    }
}

/* requested function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize once */
    if (!initialized) {
        for (int i = 0; i < 64; ++i)
            cur[i] = (i == 0 ? (seq_t)1 : (seq_t)0);  /* start from corner 0 */

        generate_adjacency();

        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);                       /* allocation failure   */
        cache[0] = (seq_t)1;
        max_cached = 0;
        cache_cap  = 1;
        initialized = true;
    }

    /* grow cache storage if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                       /* allocation failure   */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= max_cached)
        return cache[n];

    /* compute values up to n */
    for (size_t step = max_cached + 1; step <= (size_t)n; ++step) {
        /* compute next step */
        for (int i = 0; i < 64; ++i) nxt[i] = (seq_t)0;
        for (int i = 0; i < 64; ++i) {
            seq_t val = cur[i];
            if (!val) continue;
            int len = nbr_len[i];
            for (int k = 0; k < len; ++k) {
                int j = nbr[i][k];
                nxt[j] += val;
            }
        }

        /* copy next to current for next iteration */
        for (int i = 0; i < 64; ++i) cur[i] = nxt[i];

        /* store result for this step */
        cache[step] = cur[0];
    }

    max_cached = (size_t)n;
    return cache[n];
}
