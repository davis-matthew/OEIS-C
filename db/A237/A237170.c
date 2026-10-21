/*
 * A237170  Number of (n+1)X(2+1) 0..3 arrays with the maximum minus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   diff_of_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows, grouping by the constant
 * difference d = max – lower median of each 2×2 subblock.  For every
 * possible d∈{0,1,2,3} the transition graph between 3‑tuples (rows) is
 * pre‑computed.  The number of allowed (n+1)×3 arrays is the total
 * number of paths of length n in all four graphs.  No closed formula
 * is known. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint8_t */
#include "../utils/diff_of_block.h"

/* Pre‑computed adjacency lists for each difference d */
static int next_rows[4][64][64];
static uint8_t next_len[4][64];
static bool initialized = false;

static void init_adj(void)
{
    if (initialized) return;
    initialized = true;

    /* build adjacency lists */
    for (int p = 0; p < 64; ++p) {
        int a = (p >> 4) & 3;
        int b = (p >> 2) & 3;
        int c = p & 3;
        for (int q = 0; q < 64; ++q) {
            int a2 = (q >> 4) & 3;
            int b2 = (q >> 2) & 3;
            int c2 = q & 3;
            int d0 = diff_of_block(a, b, a2, b2);   /* columns 0–1 */
            int d1 = diff_of_block(b, c, b2, c2);   /* columns 1–2 */
            if (d0 == d1) {
                int d = d0;
                uint8_t len = next_len[d][p];
                next_rows[d][p][len++] = q;
                next_len[d][p] = len;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_adj();

    idx_t steps = n;                /* n transitions == (n+1)×3 arrays */
    seq_t total = (seq_t)0;

    static seq_t v0[64], v1[64];

    for (int d = 0; d < 4; ++d) {
        /* initial count: each row is admissible once */
        for (int i = 0; i < 64; ++i) v0[i] = (seq_t)1;

        seq_t *cur = v0, *nxt = v1;

        for (idx_t step = 0; step < steps; ++step) {
            /* zero next vector */
            for (int i = 0; i < 64; ++i) nxt[i] = (seq_t)0;

            for (int p = 0; p < 64; ++p) {
                seq_t val = cur[p];
                if (val == (seq_t)0) continue;
                uint8_t len = next_len[d][p];
                for (int t = 0; t < len; ++t) {
                    int q = next_rows[d][p][t];
                    nxt[q] += val;
                }
            }
            /* swap vectors for next iteration */
            seq_t *tmp = cur; cur = nxt; nxt = tmp;
        }

        /* add contribution of this difference value d */
        for (int i = 0; i < 64; ++i) total += cur[i];
    }

    return total;
}
