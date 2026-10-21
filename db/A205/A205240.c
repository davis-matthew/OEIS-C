/*
 * A205240  Number of (n+1)X3 0..2 arrays with the number of rightwards and
 *          downwards edge increases in each 2X2 subblock differing from the
 *          number in each of its horizontal and vertical neighbors.
 * https://oeis.org/A205240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the (n+1)×3 arrays of 0,1,2 with the 2×2 subblocks’
 *            increase‑counts differing from each of their horizontal and
 *            vertical neighbors. The implementation uses a DP over rows,
 *            iterating over all 27 possible rows (3³) and 25 possible
 *            sub‑block sum values (0…4). Public interface: seq_t a(id...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define ROWS 27         /* 3^3 possible rows */
#define SW   5          /* subblock sum values 0..4 */
#define STATEC (ROWS*SW*SW)

static inline int encode_state(int row, int s0, int s1)
{
    /* row ∈ [0,26], s0,s1 ∈ [0,4]  -> 0 ≤ idx < 675  */
    return row * SW * SW + s0 * SW + s1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 – undefined for 0 */

    /* static data that is computed once  */
    static bool built = false;

    /* Each row is a 3‑tuple of digits 0–2 */
    static unsigned char rowval[ROWS][3];
    /* Sum of increases for the two left/right subblocks
       between any ordered pair of rows.  */
    static unsigned char sum0[ROWS][ROWS];
    static unsigned char sum1[ROWS][ROWS];

    if (!built)
    {
        /* enumerate all rows in base‑3 notation */
        for (int r = 0; r < ROWS; ++r)
        {
            rowval[r][0] = r % 3;
            rowval[r][1] = (r / 3) % 3;
            rowval[r][2] = (r / 9) % 3;
        }

        /* pre‑compute the two subblock sums for each pair of rows */
        for (int a = 0; a < ROWS; ++a)
        {
            unsigned char p0 = rowval[a][0], p1 = rowval[a][1], p2 = rowval[a][2];
            for (int b = 0; b < ROWS; ++b)
            {
                unsigned char q0 = rowval[b][0], q1 = rowval[b][1], q2 = rowval[b][2];

                /* left subblock (columns 0‑1) */
                unsigned char s0 = (p1 > p0) + (q1 > q0) + (q0 > p0) + (q1 > p1);
                /* right subblock (columns 1‑2) */
                unsigned char s1 = (p2 > p1) + (q2 > q1) + (q1 > p1) + (q2 > p2);

                sum0[a][b] = s0;
                sum1[a][b] = s1;
            }
        }
        built = true;
    }

    /* dynamic programming tables  */
    seq_t *dp_cur  = (seq_t *)calloc(STATEC, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATEC, sizeof(seq_t));

    /* initial pair of rows: r0 (row0) and r1 (row1) */
    for (int r0 = 0; r0 < ROWS; ++r0)
    {
        for (int r1 = 0; r1 < ROWS; ++r1)
        {
            unsigned char s0 = sum0[r0][r1];
            unsigned char s1 = sum1[r0][r1];
            if (s0 == s1)               /* horizontal neighbour must differ */
                continue;
            int idx = encode_state(r1, s0, s1);
            dp_cur[idx] += (seq_t)1;     /* one way for this r0 */
        }
    }

    /* add remaining rows (if any) */
    for (idx_t step = 1; step < n; ++step)
    {
        /* zero the next table */
        for (int i = 0; i < STATEC; ++i)
            dp_next[i] = 0;

        for (int src = 0; src < STATEC; ++src)
        {
            if (!dp_cur[src])
                continue;

            int prev_row = src / (SW * SW);
            int rem      = src % (SW * SW);
            int s0_prev  = rem / SW;
            int s1_prev  = rem % SW;

            for (int nxt = 0; nxt < ROWS; ++nxt)
            {
                unsigned char ns0 = sum0[prev_row][nxt];
                unsigned char ns1 = sum1[prev_row][nxt];

                if (ns0 == ns1)                  /* horizontal diff required */
                    continue;
                if (ns0 == s0_prev || ns1 == s1_prev)
                    continue;                  /* vertical neighbour must differ */

                int dst = encode_state(nxt, ns0, ns1);
                dp_next[dst] += dp_cur[src];
            }
        }

        /* swap current and next tables for next iteration */
        seq_t *t = dp_cur;
        dp_cur   = dp_next;
        dp_next  = t;
    }

    /* sum all states to get the final count */
    seq_t result = 0;
    for (int i = 0; i < STATEC; ++i)
        result += dp_cur[i];

    free(dp_cur);
    free(dp_next);

    return result;
}
