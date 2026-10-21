/*
 * A090572  Number of configurations of the 3-dimensional 2 X 2 X 2 sliding
 *          cube puzzle that require a minimum of n moves to be reached.
 * https://oeis.org/A090572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of reachable configurations of the 2×2×2
 *            sliding‑cube puzzle that require exactly n moves (0‑based)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* factorial table for 0 … 8, values fit in a 32‑bit integer. */
static const uint32_t fact_table[9] = {1,1,2,6,24,120,720,5040,40320};

/* convert a permutation of the 8 positions (0 = empty, 1…7 = tiles) into a unique index 0…40319 */
static idx_t perm_to_index(const unsigned char *p)
{
    idx_t idx = 0;
    for (idx_t i = 0; i < 8; ++i) {
        unsigned int rank = 0;
        for (idx_t j = i + 1; j < 8; ++j) {
            if (p[j] < p[i]) {
                ++rank;
            }
        }
        idx += (idx_t)rank * fact_table[7 - i];
    }
    return idx;
}

/* static state: distance array and frequency table */
static bool initialized = false;
static uint8_t dist[40320];
static seq_t freq[50];  /* 50 is safely larger than any distance that occurs */

static void precompute(void)
{
    /* start state: tiles 1…7 in order, empty at position 7 */
    unsigned char start[8];
    for (int i = 0; i < 7; ++i) start[i] = (unsigned char)(i + 1);
    start[7] = 0;

    /* queue for BFS: 40320 states max, each 8 bytes */
    unsigned char *queue = (unsigned char*)malloc(40320 * 8);
    if (!queue) return; /* in a real environment we should handle this */

    idx_t head = 0;
    idx_t tail = 0;

    /* initialise dist to 255 = -1 (unvisited) */
    memset(dist, 0xFF, sizeof(dist));

    /* enqueue start state */
    memcpy(queue + tail * 8, start, 8);
    idx_t start_idx = perm_to_index(start);
    dist[start_idx] = 0;
    ++tail;

    /* BFS */
    while (head < tail) {
        unsigned char *curr = queue + head * 8;
        idx_t cur_idx = perm_to_index(curr);
        uint8_t cur_dist = dist[cur_idx];

        /* find empty position */
        idx_t empty = 0;
        while (curr[empty] != 0) ++empty;

        /* generate four possible moves by toggling one bit of the empty position */
        for (int bit = 0; bit < 3; ++bit) {
            idx_t newpos = empty ^ (1UL << bit);
            unsigned char next[8];
            memcpy(next, curr, 8);
            /* swap empty (0) with the tile in newpos */
            unsigned char tmp = next[empty];
            next[empty] = next[newpos];
            next[newpos] = tmp;

            idx_t next_idx = perm_to_index(next);
            if (dist[next_idx] == 0xFF) {   /* unvisited */
                dist[next_idx] = (uint8_t)(cur_dist + 1);
                memcpy(queue + tail * 8, next, 8);
                ++tail;
            }
        }

        ++head;
    }

    /* build frequency table */
    for (size_t i = 0; i < sizeof(freq)/sizeof(freq[0]); ++i) freq[i] = 0;
    for (size_t i = 0; i < 40320; ++i) {
        uint8_t d = dist[i];
        if (d != 0xFF && d < sizeof(freq)/sizeof(freq[0])) {
            freq[d] += (seq_t)1;
        }
    }

    free(queue);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }
    if (n < sizeof(freq)/sizeof(freq[0])) {
        return freq[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
