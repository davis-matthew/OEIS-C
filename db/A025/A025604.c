/*
 * A025604  Number of n-move queen paths on 8x8 board from given corner to
 *          same corner.
 * https://oeis.org/A025604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025604: number of n‑move queen paths on the 8×8 board from a given corner to the same corner.
   Computed by iteratively multiplying the 64×64 adjacency matrix of the queen graph.
   a(0)=1; for n>0, a(n) is the number of n‑step walks that start and end on the same corner.
   The function memoises all values up to the requested n. */

#include <stdlib.h>
#include <stdbool.h>

/* External types */
extern seq_t a(idx_t n);                /* declaration to satisfy some compilers */

seq_t a(idx_t n)
{
    /* Static data structures for memoisation and graph representation */
    static bool initialized = false;
    static seq_t *cache   = NULL;      /* cache of a(k) values           */
    static idx_t  cache_len = 0;       /* number of cached values        */
    static size_t cache_cap = 0;       /* allocated capacity of cache    */
    static int neighbor_cnt[64];       /* number of neighbours of each square */
    static int  neighbors[64][63];     /* neighbour indices              */
    static seq_t v_cur[64];            /* vector of counts after k moves   */
    static seq_t v_next[64];           /* vector of counts after k+1 moves */

    /* Build the queen graph adjacency lists once */
    if (!initialized) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                int idx = r * 8 + c;
                neighbor_cnt[idx] = 0;
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        if (r == r2 && c == c2) continue;
                        int dr = r - r2;
                        int dc = c - c2;
                        if (dr == 0 || dc == 0 || dr == dc || dr == -dc) {
                            int idx2 = r2 * 8 + c2;
                            if (neighbor_cnt[idx] < 63)   /* safety, though max 63 */
                                neighbors[idx][neighbor_cnt[idx]++] = idx2;
                        }
                    }
                }
            }
        }

        /* Initialise cache with a(0) = 1 */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;

        /* Initialise the vector for 0 moves (start at corner 0) */
        for (int i = 0; i < 64; ++i)
            v_cur[i] = (seq_t)0;
        v_cur[0] = (seq_t)1;

        initialized = true;
    }

    /* Extend the cache up to a(n) if necessary */
    while (cache_len <= n) {
        /* Compute v_next = A * v_cur */
        for (int i = 0; i < 64; ++i)
            v_next[i] = (seq_t)0;
        for (int p = 0; p < 64; ++p) {
            seq_t val = v_cur[p];
            if (val == (seq_t)0)
                continue;
            int cnt = neighbor_cnt[p];
            for (int k = 0; k < cnt; ++k) {
                int q = neighbors[p][k];
                v_next[q] += val;        /* addition modulo seq_t width */
            }
        }

        /* Store a(current cache_len) = v_next[0]  */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* In a real program we would handle allocation failure,
                   but for this short routine we simply exit. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = v_next[0];
        ++cache_len;

        /* Prepare for next iteration */
        for (int i = 0; i < 64; ++i)
            v_cur[i] = v_next[i];
    }

    return cache[n];
}
