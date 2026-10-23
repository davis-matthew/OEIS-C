/*
 * A183321  Number of nX9 binary arrays with an element zero only if there are
 *          an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A183321 by dynamic programming over column‑parity states.
   Each state is the 9-bit vector of parities of the 9 columns after
   processing a given number of rows.  */
#include <stddef.h>

static int  edges[512][512];          /* next states from every state */
static unsigned char edge_count[512]; /* number of outgoing edges */

/* Build the transition graph once */
static void build_graph(void)
{
    static int built = 0;
    if (built) return;

    for (int V = 0; V < 512; ++V) {
        edge_count[V] = 0;
        for (int R = 0; R < 512; ++R) {
            /* R may have 1s only where V already has 1s */
            if ((V & R) != V) continue;

            int parity = 0;
            int ok = 1;
            for (int j = 0; j < 9; ++j) {
                int bit = (R >> j) & 1;
                if (bit == 0) {
                    if (parity) { ok = 0; break; }
                } else {
                    parity ^= 1;
                }
            }
            if (!ok) continue;

            int Vp = V ^ R;              /* new column parity after this row */
            edges[V][edge_count[V]++] = Vp;
        }
    }
    built = 1;
}

seq_t a(idx_t n)
{
    build_graph();

    seq_t dp[512];
    seq_t dp_next[512];

    /* initialise dp to zero */
    for (int i = 0; i < 512; ++i) dp[i] = 0;
    dp[0] = 1;                       /* all columns even at start */

    for (idx_t r = 0; r < n; ++r) {
        /* reset dp_next to zero */
        for (int i = 0; i < 512; ++i) dp_next[i] = 0;

        for (int V = 0; V < 512; ++V) {
            seq_t ways = dp[V];
            if (ways == 0) continue;
            unsigned char ec = edge_count[V];
            for (unsigned char e = 0; e < ec; ++e) {
                int Vp = edges[V][e];
                dp_next[Vp] += ways;
            }
        }

        /* copy dp_next into dp */
        for (int i = 0; i < 512; ++i) dp[i] = dp_next[i];
    }

    /* sum over all final parities */
    seq_t total = 0;
    for (int i = 0; i < 512; ++i) total += dp[i];
    return total;
}
