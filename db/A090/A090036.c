/*
 * A090036  Number of configurations of the 5 X 2 variant of Sam Loyd's
 *          sliding block 15-puzzle that require a minimum of n moves to be
 *          reached, starting with the empty square in one of the corners.
 * https://oeis.org/A090036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   inc_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distribution of minimal move counts from the corner start state
   in the 5×2 sliding‑block puzzle (A090036). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/inc_count.h"

/* factorials up to 10! for Lehmer code */
static const idx_t fact[11] = {
    1ULL,        /* 0! */
    1ULL,        /* 1! */
    2ULL,        /* 2! */
    6ULL,        /* 3! */
    24ULL,       /* 4! */
    120ULL,      /* 5! */
    720ULL,      /* 6! */
    5040ULL,     /* 7! */
    40320ULL,    /* 8! */
    362880ULL,   /* 9! */
    3628800ULL   /*10! */
};

/* Map a permutation of 0..9 to a unique index [0,10!-1] using Lehmer code */
static idx_t perm_to_index(const uint8_t perm[10]) {
    idx_t idx = 0;
    bool used[10] = {false};
    for (int i = 0; i < 10; ++i) {
        int less = 0;
        for (int j = 0; j < perm[i]; ++j)
            if (!used[j]) less++;
        idx += (idx_t)less * fact[9 - i];
        used[perm[i]] = true;
    }
    return idx;
}

/* Decode an index into a permutation of 0..9 */
static void index_to_perm(idx_t idx, uint8_t perm[10]) {
    bool used[10] = {false};
    for (int i = 0; i < 10; ++i) {
        idx_t f = fact[9 - i];
        int val = (int)(idx / f);
        idx %= f;
        int count = -1;
        for (int x = 0; x < 10; ++x) {
            if (!used[x]) {
                ++count;
                if (count == val) {
                    perm[i] = (uint8_t)x;
                    used[x] = true;
                    break;
                }
            }
        }
    }
}

/* Neighbor information for positions 0..9 (row‑major 5×2 board) */
static const int neigh_count[10] = {2,3,3,3,2,2,3,3,3,2};
static const int neighbours[10][4] = {
    {1,5, -1,-1},   /* 0 */
    {0,2,6, -1},    /* 1 */
    {1,3,7, -1},    /* 2 */
    {2,4,8, -1},    /* 3 */
    {3,9, -1,-1},   /* 4 */
    {0,6, -1,-1},   /* 5 */
    {1,5,7, -1},    /* 6 */
    {2,6,8, -1},    /* 7 */
    {3,7,9, -1},    /* 8 */
    {4,8, -1,-1}    /* 9 */
};

#define QUEUE_CAP 3628800ULL      /* 10! */
#define DIST_UNVISITED 255U

static bool initialized = false;
static seq_t *counts = NULL;
static idx_t max_dist = 0;

seq_t a(idx_t n) {
    if (!initialized) {
        /* ---------- BFS to explore state space ---------- */
        uint8_t *distance = (uint8_t*)malloc(QUEUE_CAP * sizeof(uint8_t));
        idx_t *queue     = (idx_t*)malloc(QUEUE_CAP * sizeof(idx_t));
        if (!distance || !queue) return (seq_t)0;   /* allocation failure */

        memset(distance, DIST_UNVISITED, QUEUE_CAP);
        idx_t head = 0, tail = 0;

        /* starting position: numbers 1..9, blank at corner (index 9) */
        uint8_t start_perm[10] = {1,2,3,4,5,6,7,8,9,0};
        idx_t start_idx = perm_to_index(start_perm);
        distance[start_idx] = 0;
        queue[tail++] = start_idx;

        max_dist = 0;

        uint8_t curr_perm[10];
        uint8_t new_perm[10];

        while (head < tail) {
            idx_t cur = queue[head++];
            uint8_t cur_dist = distance[cur];
            if (cur_dist > max_dist) max_dist = cur_dist;

            index_to_perm(cur, curr_perm);

            int blank = -1;
            for (int i = 0; i < 10; ++i) {
                if (curr_perm[i] == 0) { blank = i; break; }
            }

            for (int k = 0; k < neigh_count[blank]; ++k) {
                int nb = neighbours[blank][k];
                if (nb < 0) break;  /* padding */
                memcpy(new_perm, curr_perm, 10);
                uint8_t tmp = new_perm[blank];
                new_perm[blank] = new_perm[nb];
                new_perm[nb] = tmp;
                idx_t neighbor_idx = perm_to_index(new_perm);
                if (distance[neighbor_idx] == DIST_UNVISITED) {
                    distance[neighbor_idx] = cur_dist + 1;
                    queue[tail++] = neighbor_idx;
                }
            }
        }

        /* ---------- Count distribution of distances ---------- */
        counts = (seq_t*)malloc((max_dist + 1) * sizeof(seq_t));
        if (!counts) { free(distance); free(queue); return (seq_t)0; }
        for (idx_t i = 0; i <= max_dist; ++i) counts[i] = (seq_t)0;

        for (idx_t idx = 0; idx < QUEUE_CAP; ++idx) {
            uint8_t d = distance[idx];
            if (d != DIST_UNVISITED)
                inc_count(counts, d);
        }

        free(distance);
        free(queue);

        initialized = true;
    }

    return (n <= max_dist) ? counts[n] : (seq_t)0;
}
