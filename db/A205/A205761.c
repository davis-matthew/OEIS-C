/*
 * A205761  Number of (n+1)X2 0..3 arrays with every 2X2 subblock having
 *          nonzero determinant and having the same number of clockwise edge
 *          increases as its horizontal and vertical neighbors.
 * https://oeis.org/A205761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A205761: Number of (n+1)×2 arrays over {0,1,2,3} where every 2×2 subblock
 * has a non‑zero determinant and all subblocks have the same number of
 * clockwise edge increases.
 * 
 * Computation: For each possible number K of clockwise increases (0..4)
 * we pre‑compute the adjacency graph of 16 states (pairs (x,y)).  
 * An edge s→t is allowed iff det≠0 and the block defined by s and t
 * contains exactly K clockwise increases.
 * For a given n we count walks of length n in each graph (the number of
 * rows is n+1) and sum over K.  This yields the desired sequence value.
 */

#include <stdint.h>   /* for size_t, if needed */

static int trans_count[5][16];
static int trans_to[5][16][16];
static int trans_ready = 0;

static void init_trans(void)
{
    if (trans_ready) return;
    for (int k = 0; k <= 4; ++k)
        for (int s = 0; s < 16; ++s)
            trans_count[k][s] = 0;
    for (int a = 0; a < 4; ++a)            /* top row first cell   */
        for (int b = 0; b < 4; ++b)        /* top row second cell  */
            for (int c = 0; c < 4; ++c)    /* bottom row first cell*/
                for (int d = 0; d < 4; ++d)/* bottom row second cell*/
                {
                    int s = a * 4 + b;
                    int t = c * 4 + d;
                    int det = a * d - b * c;
                    if (det == 0) continue;                /* determinant zero */

                    int inc_sum =
                        (b > a) + (d > b) + (c > d) + (a > c);
                    /* inc_sum is 0..4 */
                    int pos = trans_count[inc_sum][s];
                    trans_to[inc_sum][s][pos] = t;
                    trans_count[inc_sum][s] = pos + 1;
                }
    trans_ready = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_trans();

    seq_t total = 0;

    /* For each possible clockwise‑increase count K */
    for (int K = 0; K <= 4; ++K)
    {
        seq_t cur[16];
        seq_t nxt[16];

        /* start with any first row */
        for (int s = 0; s < 16; ++s) cur[s] = 1;

        /* n blocks ⇒ n transitions */
        for (idx_t step = 0; step < n; ++step)
        {
            for (int t = 0; t < 16; ++t) nxt[t] = 0;

            for (int s = 0; s < 16; ++s)
            {
                seq_t val = cur[s];
                if (val == 0) continue;
                int cnt = trans_count[K][s];
                for (int i = 0; i < cnt; ++i)
                {
                    int t = trans_to[K][s][i];
                    nxt[t] += val;
                }
            }
            /* swap cur and nxt */
            for (int t = 0; t < 16; ++t) cur[t] = nxt[t];
        }

        /* sum over all ending states */
        seq_t sum = 0;
        for (int s = 0; s < 16; ++s) sum += cur[s];
        total += sum;
    }

    return total;
}
