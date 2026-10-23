/*
 * A025602  Number of n-move knight paths on 8x8 board from given corner to
 *          adjacent corner.
 * https://oeis.org/A025602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑move knight paths on an 8×8 board from the
 *            corner (0,0) to the adjacent corner (0,7). Computed by dynamic
 *            programming on the knight graph; results are memoised to support
 *            out‑of‑order queries
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Knight move offsets */
static const int dr[8] = { -2, -2,  2,  2, -1, -1,  1,  1 };
static const int dc[8] = { -1,  1, -1,  1, -2,  2, -2,  2 };

/* Pre‑computed adjacency lists for the 64 board squares */
static int neighbor_count[64];
static int neighbor_list[64][8];

/* Memoisation storage */
static seq_t *a_cache = NULL;   /* array of computed a(n) values */
static idx_t cache_max = 0;     /* largest n computed so far */
static bool initialized = false;

/* Current and next step vectors for dynamic programming */
static seq_t cur_vec[64];
static seq_t next_vec[64];

static void init_knight_graph(void)
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            int idx = r * 8 + c;
            int cnt = 0;
            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];
                if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                    neighbor_list[idx][cnt++] = nr * 8 + nc;
                }
            }
            neighbor_count[idx] = cnt;
        }
    }
    memset(cur_vec, 0, sizeof(cur_vec));
    cur_vec[0] = 1;               /* start at (0,0) -> index 0 */
    a_cache = (seq_t *)malloc(sizeof(seq_t) * 1);
    a_cache[0] = (seq_t)0;        /* a(0) = 0 */
    cache_max = 0;
    initialized = true;
}

/* Computes a(n) with memoised DP up to the required index */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_knight_graph();
    }
    if (n > cache_max) {
        /* Extend the cache to include up to n */
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * (n + 1));
        for (idx_t step = cache_max + 1; step <= n; ++step) {
            memset(next_vec, 0, sizeof(next_vec));
            for (int i = 0; i < 64; ++i) {
                if (cur_vec[i]) {
                    seq_t val = cur_vec[i];
                    int cnt = neighbor_count[i];
                    const int *nbr = neighbor_list[i];
                    for (int k = 0; k < cnt; ++k) {
                        next_vec[nbr[k]] += val;
                    }
                }
            }
            /* target corner is (0,7) -> index 7 */
            a_cache[step] = next_vec[7];
            memcpy(cur_vec, next_vec, sizeof(cur_vec));
        }
        cache_max = n;
    }
    return a_cache[n];
}
