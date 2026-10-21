/*
 * A080614  Consider a 3 X 3 X 3 Rubik's Cube, but allow only the moves T2,
 *          D2, F2; sequence gives number of positions that are exactly n
 *          moves from the start.
 * https://oeis.org/A080614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   number of reachable cube states exactly n moves from the start
 *            using only the moves T2, D2, F2. The state space contains 96
 *            positions; a(n) is the size of the sphere of radius n
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    unsigned char c[8];   /* corner permutation  (0‑7) */
    unsigned char e[12];  /* edge permutation    (0‑11) */
} State;

/* permutations for the three allowed moves (double turns) */
static const unsigned char T2_c[8] = { 2, 3, 0, 1, 4, 5, 6, 7 };
static const unsigned char D2_c[8] = { 0, 1, 2, 3, 6, 7, 4, 5 };
static const unsigned char F2_c[8] = { 7, 1, 2, 4, 3, 5, 6, 0 };

static const unsigned char T2_e[12] = { 2, 3, 0, 1, 4, 5, 6, 7, 8, 9, 10, 11 };
static const unsigned char D2_e[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 8, 9 };
static const unsigned char F2_e[12] = { 8, 1, 2, 3, 7, 5, 6, 4, 0, 9, 10, 11 };

/* apply one of the three moves to a state */
static void apply_move(const State *s, State *t, int move)
{
    const unsigned char *cperm;
    const unsigned char *eperm;
    if (move == 0) {          /* T2 */
        cperm = T2_c;
        eperm = T2_e;
    } else if (move == 1) {   /* D2 */
        cperm = D2_c;
        eperm = D2_e;
    } else {                  /* F2 */
        cperm = F2_c;
        eperm = F2_e;
    }

    for (int i = 0; i < 8; ++i)
        t->c[cperm[i]] = s->c[i];
    for (int i = 0; i < 12; ++i)
        t->e[eperm[i]] = s->e[i];
}

/* compute the distribution of distances once, then answer queries */
static void init_distribution(uint8_t *dist_counts, int *max_dist)
{
    /* maximum possible states */
    State visited[96];
    uint8_t dist[96];
    int head = 0, tail = 0, sz = 0;

    /* identity state */
    State identity = { {0,1,2,3,4,5,6,7},
                       {0,1,2,3,4,5,6,7,8,9,10,11} };

    visited[0] = identity;
    dist[0]   = 0;
    head = 0; tail = 1; sz = 1;
    *max_dist = 0;

    while (head < tail) {
        int cur = head++;
        uint8_t curd = dist[cur];
        if (curd > *max_dist) *max_dist = curd;

        for (int mv = 0; mv < 3; ++mv) {
            State nxt;
            apply_move(&visited[cur], &nxt, mv);

            /* linear search for already visited state */
            int found = -1;
            for (int k = 0; k < sz; ++k) {
                if (memcmp(&visited[k], &nxt, sizeof(State)) == 0) {
                    found = k;
                    break;
                }
            }
            if (found == -1) {
                visited[sz] = nxt;
                dist[sz] = curd + 1;
                tail++;
                sz++;
            }
        }
    }

    /* compute counts for each distance */
    for (int i = 0; i <= *max_dist; ++i)
        dist_counts[i] = 0;
    for (int i = 0; i < sz; ++i)
        dist_counts[dist[i]]++;
}

/* public function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    /* 11 distances (0 … 10) are sufficient for all reachable positions */
    static uint8_t dist_counts[11];
    static int max_dist = 0;

    if (!initialized) {
        init_distribution(dist_counts, &max_dist);
        initialized = 1;
    }

    if ((idx_t)n > (idx_t)max_dist)
        return (seq_t)0;
    return (seq_t)dist_counts[(size_t)n];
}
