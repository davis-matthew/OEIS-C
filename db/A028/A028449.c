/*
 * A028449  Number of perfect matchings in graph P_{2} X P_{5} X P_{n}.
 * https://oeis.org/A028449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect matchings in graph P_2 × P_5 × P_n.
 *            Computation uses transfer‑matrix DP on 10‑vertex slices
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------- global data ----------------------------------- */

#define STATES 1024                     /* 2^10 */
static bool initialized = false;

/* adjacency mask for the 10 vertices of one layer (P_2 × P_5). */
static uint16_t neighbor_mask[10];

/* transition information: for each previous mask we store the list
   of possible next masks together with the number of ways to obtain
   that next mask. */
typedef struct {
    uint16_t next;            /* mask for the next layer           */
    seq_t    count;          /* number of ways leading to this mask */
} trans_pair_t;

static trans_pair_t transitions[STATES][STATES];
static size_t trans_len[STATES];

/* ------------------------- helper for building transitions ------------ */

static void dfs_build(unsigned int avail, unsigned int next, seq_t *counts)
{
    if (avail == 0u) {
        counts[next] += 1;
        return;
    }
    /* pick the first free vertex */
    int v = __builtin_ctz(avail);
    unsigned int avail2 = avail & ~(1u << v);

    /* (1) match v vertically to next layer */
    dfs_build(avail2, next | (1u << v), counts);

    /* (2) match v internally with a free neighbor */
    unsigned int neigh = neighbor_mask[v] & avail2;
    while (neigh) {
        int w = __builtin_ctz(neigh);
        unsigned int avail3 = avail2 & ~(1u << w);
        dfs_build(avail3, next, counts);
        neigh &= neigh - 1;
    }
}

/* Initialize neighbor masks and transition tables (once). */
static void init_all(void)
{
    /* Build adjacency for one 2×5 layer. */
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 5; ++c) {
            int i = r * 5 + c;
            unsigned int mask = 0u;
            if (c > 0)   mask |= 1u << (r * 5 + (c - 1));
            if (c < 4)   mask |= 1u << (r * 5 + (c + 1));
            if (r > 0)   mask |= 1u << ((r - 1) * 5 + c);
            if (r < 1)   mask |= 1u << ((r + 1) * 5 + c);
            neighbor_mask[i] = (uint16_t)mask;
        }
    }

    /* Pre‑compute transitions for all 1024 previous masks. */
    for (unsigned int ms = 0u; ms < STATES; ++ms) {
        seq_t counts[STATES];
        for (unsigned int i = 0u; i < STATES; ++i) counts[i] = 0;
        unsigned int avail = (STATES - 1u) ^ ms;
        dfs_build(avail, 0u, counts);

        size_t pos = 0u;
        for (unsigned int nm = 0u; nm < STATES; ++nm) {
            if (counts[nm] != 0) {
                transitions[ms][pos].next  = (uint16_t)nm;
                transitions[ms][pos].count = counts[nm];
                ++pos;
            }
        }
        trans_len[ms] = pos;
    }

    initialized = true;
}

/* ----------------------------- public function --------------------------- */

seq_t a(idx_t n)
{
    if (!initialized) init_all();

    seq_t dp_cur[STATES] = {0};
    seq_t dp_next[STATES];
    dp_cur[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* zero dp_next */
        for (unsigned int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (unsigned int prev = 0; prev < STATES; ++prev) {
            seq_t val = dp_cur[prev];
            if (val == 0) continue;
            const size_t tlen = trans_len[prev];
            for (size_t t = 0; t < tlen; ++t) {
                uint16_t nxt = transitions[prev][t].next;
                seq_t   cnt = transitions[prev][t].count;
                dp_next[nxt] += val * cnt;
            }
        }

        /* copy dp_next to dp_cur for the next iteration */
        for (unsigned int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    return dp_cur[0];
}
