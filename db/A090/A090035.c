/*
 * A090035  Number of configurations of the 4 X 2 variant of Sam Loyd's
 *          sliding block 15-puzzle that require a minimum of n moves to be
 *          reached, starting with the empty square in one of the corners.
 * https://oeis.org/A090035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: exhaustive BFS enumeration of 4×2 sliding puzzle
   states.  The function returns the number of configurations that
   are exactly n moves away from the canonical start state. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* state space constants */
#define TOTAL_STATES 40320            /* 8! */
#define MAX_DEPTH    70               /* safe upper bound for depth count array */

/* adjacency for the 4×2 grid (positions 0..7, row‑major) */
static const uint8_t neighbours[8][4] = {
    {1,4,},      /* 0  */
    {0,2,5,},   /* 1  */
    {1,3,6,},   /* 2  */
    {2,7,},     /* 3  */
    {0,5,},     /* 4  */
    {1,4,6,8},  /* 5  (8 is invalid, will be ignored) */
    {2,5,7,},   /* 6  */
    {3,6,}      /* 7  */
};
static const uint8_t deg[8] = {2,3,3,2,2,3,3,2};

/* factorial numbers for Cantor expansion */
static const uint32_t fact[9] = {
    1,1,2,6,24,120,720,5040,40320
};

/* distance array (255 = unvisited) */
static uint8_t dist[TOTAL_STATES];
static seq_t depthCount[MAX_DEPTH] = {0};

/* maximum depth found during BFS */
static idx_t maxDepth = 0;
static bool initialized = false;

/* encode a permutation of 0..7 to a unique index in [0,TOTAL_STATES) */
static int encode(const uint8_t perm[8])
{
    int idx = 0;
    for (int i = 0; i < 8; ++i) {
        int rank = 0;
        for (int j = i + 1; j < 8; ++j)
            if (perm[j] < perm[i]) ++rank;
        idx += rank * fact[7 - i];
    }
    return idx;
}

/* perform BFS once and fill dist[] and depthCount[] */
static void init(void)
{
    /* initial state: tiles 1..7, blank (0) in bottom‑right corner (pos 7) */
    uint8_t startPerm[8];
    for (int i = 0; i < 8; ++i)
        startPerm[i] = (i < 7) ? (uint8_t)(i + 1) : 0;
    int startIdx = encode(startPerm);

    /* initialise distance array to 255 */
    memset(dist, 0xFF, sizeof(dist));

    /* BFS queue */
    int queue[TOTAL_STATES];
    int head = 0, tail = 0;
    queue[tail++] = startIdx;
    dist[startIdx] = 0;
    depthCount[0] = 1;
    maxDepth = 0;

    while (head < tail) {
        int curIdx = queue[head++];
        uint8_t curPerm[8];
        /* decode current index to permutation */
        int tmp = curIdx;
        for (int i = 0; i < 8; ++i) {
            int div = fact[7 - i];
            int digit = tmp / div;
            tmp %= div;

            /* find the digit-th unused number */
            int cnt = -1;
            for (int val = 0; val < 8; ++val) {
                int used = 0;
                for (int k = 0; k < i; ++k)
                    if (curPerm[k] == val) ++used;
                if (used == 0) {
                    ++cnt;
                    if (cnt == digit) {
                        curPerm[i] = (uint8_t)val;
                        break;
                    }
                }
            }
        }

        /* locate the blank (value 0) */
        int blank = 0;
        while (curPerm[blank] != 0) ++blank;

        uint8_t curDist = dist[curIdx];
        int newDist = curDist + 1;

        /* explore neighbours */
        for (int k = 0; k < deg[blank]; ++k) {
            int nb = neighbours[blank][k];
            if (nb >= 8) continue;  /* defensive: ignore unused entries */

            uint8_t nextPerm[8];
            memcpy(nextPerm, curPerm, 8);
            /* swap blank with neighbour */
            nextPerm[blank] = nextPerm[nb];
            nextPerm[nb] = 0;

            int nextIdx = encode(nextPerm);
            if (dist[nextIdx] == 255U) {
                dist[nextIdx] = (uint8_t)newDist;
                if (newDist < MAX_DEPTH)
                    depthCount[newDist] += 1;
                if ((idx_t)newDist > maxDepth)
                    maxDepth = (idx_t)newDist;
                queue[tail++] = nextIdx;
            }
        }
    }

    initialized = true;
}

/* public function: returns a(n) */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n <= maxDepth) return depthCount[(size_t)n];
    return (seq_t)0;
}
