/*
 * A227750  Number of n X 5 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 6 binary array having determinant equal to one.
 * https://oeis.org/A227750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the A227750 sequence.
   It builds a deterministic automaton on 6‑bit rows
   that records the 5‑bit output for each 2×2 subblock.
   The number of distinct n‑row output blocks is the number
   of length‑n paths in the automaton.  The automaton
   is constructed once and reused for subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESC 32           /* maximum outgoing edges per state */
#define MAX_NODES 2048        /* sufficient for this automaton */

/* table of target states for each source and symbol */
static uint64_t targ[64][MAX_DESC];

/* adjacency tables of the automaton (compressed by multiplicity) */
static int outdeg[MAX_NODES];
static int to[MAX_NODES][MAX_DESC];
static int mult[MAX_NODES][MAX_DESC];

/* per‑node mask of 64 states and queue for BFS */
static uint64_t mask_of_node[MAX_NODES];
static int queue[MAX_NODES];

/* number of nodes in the automaton */
static int nodeCount = 0;
static int graph_built = 0;

/* build the automaton once */
static void build_graph(void)
{
    int s, t, j, sym;

    /* compute targ: for each pair (s,t) produce 5‑bit word */
    for (s = 0; s < 64; ++s) {
        for (t = 0; t < 64; ++t) {
            int w = 0;
            for (j = 0; j < 5; ++j) {
                int a = (s >> j) & 1;
                int b = (t >> j) & 1;
                int c = (s >> (j + 1)) & 1;
                int d = (t >> (j + 1)) & 1;
                if (a && d && !(b && c))
                    w |= (1 << j);
            }
            targ[s][w] |= (1ULL << t);
        }
    }

    /* BFS over subsets of 64 states, starting from the full set */
    nodeCount = 1;
    mask_of_node[0] = ~0ULL;          /* start node */
    queue[0] = 0;
    int q_front = 0, q_back = 1;
    outdeg[0] = 0;                    /* will be filled below */

    while (q_front < q_back) {
        int cur = queue[q_front++];
        uint64_t curMask = mask_of_node[cur];

        for (sym = 0; sym < 32; ++sym) {
            uint64_t nextMask = 0;
            uint64_t m = curMask;
            while (m) {
                int src = __builtin_ctzll(m);
                m &= m - 1;
                nextMask |= targ[src][sym];
            }
            if (!nextMask) continue;

            /* find or create node for nextMask */
            int nxt = -1;
            for (int k = 0; k < nodeCount; ++k) {
                if (mask_of_node[k] == nextMask) {
                    nxt = k;
                    break;
                }
            }
            if (nxt == -1) {
                nxt = nodeCount++;
                mask_of_node[nxt] = nextMask;
                outdeg[nxt] = 0;
                queue[q_back++] = nxt;
            }

            /* add transition cur -> nxt, increment multiplicity if needed */
            int k;
            for (k = 0; k < outdeg[cur]; ++k) {
                if (to[cur][k] == nxt) {
                    ++mult[cur][k];
                    break;
                }
            }
            if (k == outdeg[cur]) {        /* new transition */
                to[cur][outdeg[cur]] = nxt;
                mult[cur][outdeg[cur]] = 1;
                ++outdeg[cur];
            }
        }
    }
    graph_built = 1;
}

seq_t a(idx_t n)
{
    if (!graph_built)
        build_graph();

    if (n == 0) return (seq_t)0;

    /* dynamic arrays for DP */
    size_t sz = (size_t)nodeCount;
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !nxt) return (seq_t)0;    /* allocation failure */

    cur[0] = (seq_t)1;                    /* start node has id 0 */

    size_t step;
    for (step = 0; step < (size_t)n; ++step) {
        /* zero next vector */
        memset(nxt, 0, sz * sizeof(seq_t));

        for (size_t i = 0; i < sz; ++i) {
            if (!cur[i]) continue;
            seq_t v = cur[i];
            int od = outdeg[i];
            for (int k = 0; k < od; ++k) {
                int nb = to[i][k];
                int cnt = mult[i][k];
                seq_t prod = v;
                prod *= (seq_t)cnt;
                nxt[nb] += prod;
            }
        }
        /* swap cur and nxt */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all reachable states */
    seq_t result = 0;
    for (size_t i = 0; i < sz; ++i)
        result += cur[i];

    free(cur);
    free(nxt);
    return result;
}
