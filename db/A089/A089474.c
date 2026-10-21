/*
 * A089474  Number of configurations of the sliding block 8-puzzle that
 *          require a minimum of n moves to be reached, starting with the
 *          empty square in the center.
 * https://oeis.org/A089474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089474 – Distribution of distances from the 8‑puzzle state with
 *  the blank square in the centre.
 *
 *  a(n) = number of configurations that require *exactly* n moves
 *  to reach from the starting state
 *    1 2 3
 *    4 – 5
 *    6 7 8
 *
 *  The values are obtained by a breadth‑first search on the sliding
 *  puzzle graph.  The search is performed once and the depth counts
 *  are cached for subsequent calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* factorial values for 0! … 8! */
static const int fact[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

/* Count of states at each depth (max depth is less than 32). */
static seq_t depth_counts[64] = {0};

/* Maximum depth found by the BFS */
static int max_depth = 0;

/* Flag to indicate that the distribution has already been computed. */
static bool computed = false;

/* Convert a permutation of 0..8 to its lexicographic index [0..362879]. */
static size_t perm_to_index(const uint8_t p[9])
{
    uint8_t used[9] = {0};
    size_t idx = 0;
    for (int i = 0; i < 9; ++i) {
        uint8_t v = p[i];
        int cnt = 0;
        for (int j = 0; j < v; ++j)
            if (!used[j]) ++cnt;
        idx += cnt * fact[8 - i];
        used[v] = 1;
    }
    return idx;
}

/* Perform a BFS from the starting configuration and fill depth_counts. */
static void compute_distribution(void)
{
    /* distance array, 255 means unvisited */
    static unsigned char dist[362880];
    memset(dist, 255, sizeof(dist));

    /* Queue of states (each state is 9 bytes). */
    struct Node { uint8_t s[9]; };
    static struct Node queue[362880];
    size_t qhead = 0, qtail = 0;

    /* Starting state: 1 2 3 / 4 – 5 / 6 7 8 */
    struct Node start = { .s = {1,2,3,4,0,5,6,7,8} };

    size_t start_idx = perm_to_index(start.s);
    dist[start_idx] = 0;
    depth_counts[0] = 1;
    max_depth = 0;
    queue[qtail++] = start;

    /* Moves: up, down, left, right */
    const int drow[4] = {-1, 1, 0, 0};
    const int dcol[4] = {0, 0, -1, 1};

    while (qhead < qtail) {
        struct Node cur = queue[qhead++];
        size_t cur_idx = perm_to_index(cur.s);
        unsigned char cur_depth = dist[cur_idx];

        /* locate the blank (value 0) */
        int blank = -1;
        for (int i = 0; i < 9; ++i)
            if (cur.s[i] == 0) { blank = i; break; }

        int r = blank / 3;
        int c = blank % 3;

        for (int d = 0; d < 4; ++d) {
            int nr = r + drow[d];
            int nc = c + dcol[d];
            if (nr < 0 || nr > 2 || nc < 0 || nc > 2) continue;
            int npos = nr * 3 + nc;

            struct Node nxt = cur;
            /* swap the blank with the adjacent tile */
            nxt.s[blank] = cur.s[npos];
            nxt.s[npos] = 0;

            size_t nxt_idx = perm_to_index(nxt.s);
            if (dist[nxt_idx] == 255) {
                dist[nxt_idx] = cur_depth + 1;
                int nd = (int)(cur_depth + 1);
                depth_counts[nd] = depth_counts[nd] + 1;
                if (nd > max_depth) max_depth = nd;
                queue[qtail++] = nxt;
            }
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (!computed) {
        compute_distribution();
        computed = true;
    }
    if (n <= (idx_t)max_depth)
        return depth_counts[(size_t)n];
    else
        return (seq_t)0;
}
