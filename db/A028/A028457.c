/*
 * A028457  Number of perfect matchings in graph P_{2} X C_{5} X P_{n}.
 * https://oeis.org/A028457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028457: Number of perfect matchings in P₂ × C₅ × Pₙ.
   Implemented with a transfer‑matrix dynamic programming on the
   10‑vertex cross‑section P₂ × C₅.  The algorithm runs in O(n·2¹⁰) time
   and stores the already‑computed values in a static array.  Initial
   values are taken from the sequence description. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define V 10
#define STATE_MASK ((1 << V) - 1)

/* --------------------------------------------------------------------- */
/* adjacency of P₂ × C₅ ------------------------------------------------ */
static unsigned char neigh[V][V];

/* --------------------------------------------------------------------- */
/* matchings for a vertex mask ------------------------------------------ */
static seq_t matchings[1 << V];
static unsigned char memo_done[1 << V];

/* --------------------------------------------------------------------- */
/* structure of a transition ------------------------------------------- */
struct trans_entry { int dest; seq_t val; };
static struct trans_entry *trans_lists[1 << V];
static int trans_counts[1 << V];

/* --------------------------------------------------------------------- */
/* dynamic programming arrays ------------------------------------------- */
static seq_t dp[1 << V];
static seq_t nextdp[1 << V];
static int active_states[1 << V];
static int next_active_states[1 << V];
static int active_count = 0;

/* --------------------------------------------------------------------- */
/* cache of a(n) up to n = 10000 --------------------------------------- */
#define MAX_N 10000
static seq_t a_cache[MAX_N + 1];
static size_t a_cache_size = 0;
static idx_t cur_n = 0;
static bool initialized = false;

/* --------------------------------------------------------------------- */
/* construct adjacency matrix of P₂ × C₅ -------------------------------- */
static void init_neighbors(void)
{
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            neigh[i][j] = 0;

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 5; ++y) {
            int v = x * 5 + y;
            int v_vert = (1 - x) * 5 + y;
            neigh[v][v_vert] = neigh[v_vert][v] = 1;

            int y_left  = (y + 4) % 5;
            int y_right = (y + 1) % 5;
            int v_left  = x * 5 + y_left;
            int v_right = x * 5 + y_right;
            neigh[v][v_left]  = neigh[v_left][v] = 1;
            neigh[v][v_right] = neigh[v_right][v] = 1;
        }
    }
}

/* --------------------------------------------------------------------- */
/* count perfect matchings in an induced subgraph (mask)                  */
static seq_t count_matchings(int mask)
{
    if (mask == 0) return (seq_t)1;
    if (memo_done[mask]) return matchings[mask];

    int v = __builtin_ctz((unsigned)mask);          /* first set bit */
    int rest = mask & (~(1 << v));
    seq_t sum = 0;

    for (int u = 0; u < V; ++u) {
        if (neigh[v][u] && (rest & (1 << u))) {
            sum += count_matchings(rest & (~(1 << u)));
        }
    }

    memo_done[mask] = 1;
    matchings[mask] = sum;
    return sum;
}

/* --------------------------------------------------------------------- */
/* build transition tables ---------------------------------------------- */
static void init_transitions(void)
{
    /* Allocate space for all transition entries. */
    size_t total_entries = 0;
    for (int s = 0; s <= STATE_MASK; ++s) {
        int cnt = 0;
        for (int t = 0; t <= STATE_MASK; ++t) {
            if (s & t) continue;
            int rest = STATE_MASK ^ (s | t);
            if (matchings[rest]) ++cnt;
        }
        trans_counts[s] = cnt;
        total_entries += cnt;
    }

    struct trans_entry *block = (struct trans_entry*)malloc(total_entries * sizeof(struct trans_entry));
    size_t offset = 0;
    for (int s = 0; s <= STATE_MASK; ++s) {
        trans_lists[s] = block + offset;
        for (int t = 0; t <= STATE_MASK; ++t) {
            if (s & t) continue;
            int rest = STATE_MASK ^ (s | t);
            seq_t val = matchings[rest];
            if (val) {
                block[offset].dest = t;
                block[offset].val  = val;
                ++offset;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* one‑time initialization ---------------------------------------------- */
static void initialize(void)
{
    init_neighbors();

    /* compute matchings for all masks */
    memset(memo_done, 0, sizeof(memo_done));
    matchings[0] = 1;
    memo_done[0] = 1;
    for (int mask = 1; mask <= STATE_MASK; ++mask)
        if (!memo_done[mask]) count_matchings(mask);

    init_transitions();

    /* initial DP state for n = 0 */
    memset(dp, 0, sizeof(dp));
    dp[0] = 1;
    active_states[0] = 0;
    active_count = 1;

    a_cache[0] = dp[0];
    a_cache_size = 1;
    cur_n = 0;
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) initialize();

    if (n < cur_n) return a_cache[n];

    for (idx_t step = cur_n + 1; step <= n; ++step) {
        memset(nextdp, 0, sizeof(nextdp));
        int next_count = 0;

        for (int si = 0; si < active_count; ++si) {
            int s = active_states[si];
            seq_t d = dp[s];
            if (d == 0) continue;

            struct trans_entry *list = trans_lists[s];
            int cnt = trans_counts[s];
            for (int i = 0; i < cnt; ++i) {
                int t = list[i].dest;
                seq_t val = list[i].val;
                if (nextdp[t] == 0) next_active_states[next_count++] = t;
                nextdp[t] += d * val;
            }
        }

        memcpy(dp, nextdp, sizeof(dp));
        memcpy(active_states, next_active_states,
               sizeof(int) * next_count);
        active_count = next_count;

        if (a_cache_size <= n) a_cache[a_cache_size++] = dp[0];
    }

    cur_n = n;
    return a_cache[n];
}
