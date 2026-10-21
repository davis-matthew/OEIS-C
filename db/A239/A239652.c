/*
 * A239652  Number of 4Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n arrays of 0..3 entries such that for each
 *            cell the value is not equal to (sum of elements to its left mod
 *            4) nor to (sum of elements above it + 2 mod 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>   /* for memset */

/* Edge structure used for the adjacency list of the column transition graph. */
typedef struct {
    uint8_t next;        /* next state (0..255) */
    uint16_t cnt;        /* number of ways to go from current state to `next` */
} Edge;

static Edge  edges[65536];          /* maximum 256×256 edges */
static int   edge_start[256];       /* index of first edge for each state */
static int   edge_end[256];         /* index after last edge for each state */
static int   initialized = 0;       /* flag for one‑time adjacency construction */

/* DP arrays.  Static to avoid repeated allocation. */
static seq_t dp_cur[256];
static seq_t dp_next[256];

seq_t a(idx_t n) {
    /* --------------------------------------------------------------------
       Build the transition adjacency list (once per process).
       Each state encodes the cumulative sum modulo 4 of each row
       up to the previous column, as a 4‑digit base‑4 number.
       -------------------------------------------------------------------- */
    if (!initialized) {
        int edge_index = 0;

        for (int state = 0; state < 256; ++state) {
            int start = edge_index;      /* edges for this state start here */

            /* Separate the four left sums (0..3) from `state`. */
            uint8_t left[4];
            int tmp = state;
            for (int r = 0; r < 4; ++r) {
                left[r] = tmp & 3;
                tmp >>= 2;
            }

            /* Temporary counter for how many assignments lead to each
               new state from this `state`.  Limited to 256, so uint16_t works. */
            uint16_t cnt[256];
            memset(cnt, 0, sizeof(cnt));

            /* Enumerate all 4‑tuple assignments (0..3)^4. */
            for (int mask = 0; mask < 256; ++mask) {
                uint8_t val[4];
                int t = mask;
                for (int r = 0; r < 4; ++r) {
                    val[r] = t & 3;
                    t >>= 2;
                }

                /* Check the two forbidden values for every row. */
                uint8_t up = 0;            /* cumulative sum above in this column */
                int ok = 1;
                for (int r = 0; r < 4 && ok; ++r) {
                    uint8_t forb1 = left[r];
                    uint8_t forb2 = (up + 2u) & 3u;  /* (sum_above + 2) mod 4 */
                    if (val[r] == forb1 || val[r] == forb2)
                        ok = 0;
                    up = (up + val[r]) & 3u;
                }
                if (!ok) continue;   /* this assignment is illegal */

                /* Compute the new left sums after inserting this column. */
                uint8_t new_left[4];
                for (int r = 0; r < 4; ++r)
                    new_left[r] = (left[r] + val[r]) & 3u;

                /* Encode the new state. */
                uint8_t new_state = new_left[0]
                                 | (new_left[1] << 2)
                                 | (new_left[2] << 4)
                                 | (new_left[3] << 6);

                ++cnt[new_state];
            }

            /* Flush the counters into the global edge array. */
            for (int s = 0; s < 256; ++s) {
                if (cnt[s] != 0) {
                    edges[edge_index].next = (uint8_t)s;
                    edges[edge_index].cnt  = cnt[s];
                    ++edge_index;
                }
            }
            edge_start[state] = start;
            edge_end[state]   = edge_index;
        }
        initialized = 1;
    }

    /* --------------------------------------------------------------------
       DP over columns.  dp_cur holds the number of ways to reach each
       left‑sum state after processing a certain number of columns.
       -------------------------------------------------------------------- */
    memset(dp_cur, 0, sizeof(dp_cur));
    dp_cur[0] = 1;        /* all left sums start at zero */

    for (idx_t col = 0; col < n; ++col) {
        /* Reset the next column's DP array. */
        memset(dp_next, 0, sizeof(dp_next));

        /* For each current state, jump to all reachable next states. */
        for (int cur = 0; cur < 256; ++cur) {
            seq_t ways = dp_cur[cur];
            if (!ways) continue;          /* skip zero entries */

            for (int ei = edge_start[cur]; ei < edge_end[cur]; ++ei) {
                const Edge *e = &edges[ei];
                /* dp_next[next] += ways * cnt */
                dp_next[e->next] = dp_next[e->next] + (ways * (seq_t)e->cnt);
            }
        }
        /* Copy dp_next back into dp_cur for the next column. */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* The answer is the sum over all final left‑sum states. */
    seq_t total = 0;
    for (int s = 0; s < 256; ++s)
        total = total + dp_cur[s];
    return total;
}
