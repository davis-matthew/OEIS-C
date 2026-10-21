/*
 * A089473  Number of configurations of the sliding block 8-puzzle that
 *          require a minimum of n moves to be reached, starting with the
 *          empty square in one of the corners.
 * https://oeis.org/A089473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   idx_rc, is_visited, set_visited
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = number of distinct 8‑puzzle configurations that
 * require exactly n moves from a corner start (empty square in a corner).
 * Computed by a breadth‑first search over all 181 440 reachable states,
 * recording the distribution of minimal distances. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* strchr, memset */
#include "../utils/idx_rc.h"
#include "../utils/is_visited.h"
#include "../utils/set_visited.h"

/* external types supplied by the build system */
typedef uint64_t idx_t;   /* index type */

/* function prototype */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Constants                                                            */
/* ------------------------------------------------------------------ */
#define MAX_POS       362880   /* 9!  */
#define MAX_NODES     181440   /* 9!/2, reachable states */
#define MAX_DIST      32       /* distances 0..31 */
#define VISITED_SIZE  ((MAX_POS + 7) / 8)   /* byte array for bitset */

/* ------------------------------------------------------------------ */
/* Utilities                                                             */

/* factorials up to 9! */
static const int fact[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

/* Cantor expansion: rank of a permutation of 0..8 */
static int perm_rank(const uint8_t p[9])
{
    int r = 0;
    for (int i = 0; i < 9; ++i) {
        int less = 0;
        for (int j = i + 1; j < 9; ++j)
            if (p[j] < p[i]) ++less;
        r += less * fact[8 - i];
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Node definition                                                      */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t state[9];
    uint8_t dist;
} Node;

/* ------------------------------------------------------------------ */
/* Main function                                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t counts[MAX_DIST];
    /* local static storage for the search */
    static uint8_t visited[VISITED_SIZE];
    static Node queue[MAX_NODES];
    int head = 0, tail = 0;

    if (!built) {
        /* clear visited bitset and counts */
        memset(visited, 0, sizeof(visited));
        memset(counts, 0, sizeof(counts));

        /* start configuration: empty square (0) at bottom‑right corner */
        static const uint8_t start_state[9] = {1,2,3,4,5,6,7,8,0};
        int start_rank = perm_rank(start_state);

        set_visited(visited, start_rank);

        /* enqueue start */
        for (int i = 0; i < 9; ++i) queue[tail].state[i] = start_state[i];
        queue[tail].dist = 0;
        ++tail;
        counts[0] = 1;          /* a(0) = 1 */

        /* BFS */
        const int dr[4] = {-1, 1, 0, 0};
        const int dc[4] = {0, 0, -1, 1};

        while (head < tail) {
            Node *cur = &queue[head++];
            const uint8_t *s = cur->state;
            uint8_t cur_dist = cur->dist;

            /* locate the empty slot */
            int zero = 0;
            while (s[zero] != 0) ++zero;
            int r = zero / 3, c = zero % 3;

            /* explore moves */
            for (int dir = 0; dir < 4; ++dir) {
                int nr = r + dr[dir], nc = c + dc[dir];
                if (nr < 0 || nr > 2 || nc < 0 || nc > 2) continue;
                int npos = idx_rc(nr, nc);

                uint8_t new_state[9];
                memcpy(new_state, s, 9);
                uint8_t tmp = new_state[zero];
                new_state[zero] = new_state[npos];
                new_state[npos] = tmp;

                int rank = perm_rank(new_state);
                if (!is_visited(visited, rank)) {
                    set_visited(visited, rank);
                    for (int k = 0; k < 9; ++k) queue[tail].state[k] = new_state[k];
                    queue[tail].dist = (uint8_t)(cur_dist + 1);
                    ++tail;
                    if ((uint32_t)cur_dist + 1 < MAX_DIST)
                        ++counts[(int)(cur_dist + 1)];
                }
            }
        }
        built = true;
    }

    if (n >= MAX_DIST) return (seq_t)0;
    return counts[(int)n];
}
