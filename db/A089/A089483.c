/*
 * A089483  Number of configurations of the sliding block 8-puzzle that
 *          require a minimum of n moves to be reached, starting with the
 *          empty square at mid-side.
 * https://oeis.org/A089483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of 8‑puzzle configurations that are
   exactly n moves from the start position with the blank in the
   middle of a side. */

/* The algorithm performs a single BFS over the 9! puzzle graph once
   and stores the distribution of distances.  Subsequent calls
   retrieve the precomputed count. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* factorials up to 9! (needed for permutation ranking). */
static const idx_t FACT[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
#define NUM_STATES FACT[9]           /* 9! = 362880 */

/* Rank a permutation of 0..8 into the range [0,9!-1] using Lehmer code. */
static inline idx_t perm_rank(const uint8_t p[9]) {
    idx_t r = 0;
    bool used[9] = {false};
    for (int i = 0; i < 9; ++i) {
        int cnt = 0, d = p[i];
        for (int j = 0; j < d; ++j)
            if (!used[j]) ++cnt;
        r += (idx_t)cnt * FACT[8 - i];
        used[d] = true;
    }
    return r;
}

/* Unrank: convert index back to permutation. */
static inline void perm_unrank(idx_t r, uint8_t p[9]) {
    bool used[9] = {false};
    for (int i = 0; i < 9; ++i) {
        idx_t f = FACT[8 - i];
        int cnt = r / f;
        r %= f;
        int d;
        for (d = 0; d < 9; ++d)
            if (!used[d]) {
                if (cnt == 0) break;
                --cnt;
            }
        p[i] = (uint8_t)d;
        used[d] = true;
    }
}

/* Main function: returns a(n) for all n >= 0. */
seq_t a(idx_t n) {
    static bool ready = false;
    static uint8_t *dist = NULL;        /* distance for each state (0..255)   */
    static idx_t *queue = NULL;         /* BFS queue (ranks)                */
    static seq_t *freq = NULL;          /* frequency array (distance -> count) */
    static idx_t max_dist = 0;          /* maximum distance found            */

    if (!ready) {
        /* Allocate distance array and BFS queue. */
        dist = (uint8_t *)malloc(NUM_STATES * sizeof(uint8_t));
        queue = (idx_t *)malloc(NUM_STATES * sizeof(idx_t));
        if (!dist || !queue) return (seq_t)0;   /* unlikely to happen */

        /* Initialize all distances to sentinel 255. */
        memset(dist, 0xFF, NUM_STATES * sizeof(uint8_t));

        /* Start state: 1 0 2 / 3 4 5 / 6 7 8  (blank at top middle). */
        uint8_t start_perm[9] = {1, 0, 2, 3, 4, 5, 6, 7, 8};
        idx_t start_rank = perm_rank(start_perm);

        /* BFS initialization. */
        idx_t head = 0, tail = 0;
        queue[tail++] = start_rank;
        dist[start_rank] = 0;
        max_dist = 0;

        /* BFS over the puzzle graph. */
        while (head < tail) {
            idx_t cur_rank = queue[head++];
            uint8_t cur_perm[9];
            perm_unrank(cur_rank, cur_perm);

            /* Locate the blank (digit 0). */
            int blank = 0;
            while (cur_perm[blank] != 0) ++blank;
            int row = blank / 3;
            int col = blank % 3;

            /* Generate up to four neighbor states. */
            const int dr[4] = {-1, 1, 0, 0};
            const int dc[4] = {0, 0, -1, 1};
            for (int d = 0; d < 4; ++d) {
                int nr = row + dr[d];
                int nc = col + dc[d];
                if (nr < 0 || nr > 2 || nc < 0 || nc > 2) continue;
                int nidx = nr * 3 + nc;
                uint8_t neigh_perm[9];
                memcpy(neigh_perm, cur_perm, 9);
                uint8_t tmp = neigh_perm[blank];
                neigh_perm[blank] = neigh_perm[nidx];
                neigh_perm[nidx] = tmp;
                idx_t neigh_rank = perm_rank(neigh_perm);
                if (dist[neigh_rank] == 0xFF) {
                    dist[neigh_rank] = dist[cur_rank] + 1;
                    if ((idx_t)dist[neigh_rank] > max_dist) max_dist = dist[neigh_rank];
                    queue[tail++] = neigh_rank;
                }
            }
        }

        /* Count frequencies of each distance. */
        freq = (seq_t *)malloc((max_dist + 1) * sizeof(seq_t));
        if (!freq) return (seq_t)0;  /* unlikely to happen */
        for (idx_t i = 0; i <= max_dist; ++i) freq[i] = 0;
        for (idx_t i = 0; i < NUM_STATES; ++i) {
            uint8_t d = dist[i];
            if (d != 0xFF) freq[d] = (seq_t)(freq[d] + (seq_t)1);
        }

        ready = true;
    }

    if (n > max_dist) return (seq_t)0;
    return freq[n];
}
