/*
 * A123518  Number of dumbbells in all possible arrangements of dumbbells on a
 *          2 X n rectangular array of compartments.
 * https://oeis.org/A123518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k*A046741(n,k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the total number of dumbbells (dominoes) in all matchings
   of a 2 × n board.  The algorithm uses a column‑by‑column DP over
   all 4 possible states of a column (bitmask of cells already matched
   to the right).  Complexity is O(n). */

#include <stdbool.h>

enum { MAX_STATES = 4, MAX_TRANS = 8 };  /* compile‑time constants */

/* Transition: from a given state to the next state's mask, number
   of configurations leading to it (always 1) and the number of edges
   contributed by that transition. */
struct Transition {
    int next;          /* mask of cells matched to the right */
    seq_t count;       /* number of local configurations (1) */
    unsigned edges;    /* horizontal matchings + optional vertical */
};

static struct Transition trans[MAX_STATES][MAX_TRANS];
static int trans_len[MAX_STATES];

/* Build the transition table once. */
static void init_transitions(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (int s = 0; s < MAX_STATES; ++s) {
        trans_len[s] = 0;
        int free_mask = (~s) & 3;           /* cells not already matched to the left */
        for (int H = 0; H < 4; ++H) {        /* subset of free cells to match to the right */
            if ((H & free_mask) != H) continue;
            int remaining = free_mask & (~H);
            int vert_opts = (remaining == 3) ? 2 : 1;  /* both free: vertical possible */
            for (int v = 0; v < vert_opts; ++v) {
                unsigned edges = __builtin_popcount((unsigned)H) + v;
                int ns = H;                  /* cells matched to the right form the next state */
                struct Transition *t = &trans[s][trans_len[s]++];
                t->next  = ns;
                t->count = (seq_t)1;         /* each option is a single configuration */
                t->edges = edges;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_transitions();

    seq_t count[MAX_STATES] = {0};
    seq_t sum[MAX_STATES]   = {0};
    count[0] = (seq_t)1;     /* start with empty state, no matchings yet */

    for (idx_t col = 0; col < n; ++col) {
        seq_t new_count[MAX_STATES] = {0};
        seq_t new_sum[MAX_STATES]   = {0};

        for (int s = 0; s < MAX_STATES; ++s) {
            if (count[s] == 0) continue;

            seq_t cnt  = count[s];
            seq_t ssum = sum[s];

            for (int t = 0; t < trans_len[s]; ++t) {
                int ns   = trans[s][t].next;
                seq_t add_cnt  = cnt * trans[s][t].count;      /* = cnt */
                seq_t add_sum  = ssum * trans[s][t].count
                               + cnt * (seq_t)trans[s][t].edges;

                new_count[ns] += add_cnt;
                new_sum[ns]   += add_sum;
            }
        }

        for (int s = 0; s < MAX_STATES; ++s) {
            count[s] = new_count[s];
            sum[s]   = new_sum[s];
        }
    }

    return sum[0];   /* final state 0: no pending right matches */
}
