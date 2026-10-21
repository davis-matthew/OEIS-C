/*
 * A233812  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having the sum of the squares of all six edge and diagonal
 *          differences equal to 11.
 * https://oeis.org/A233812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries in {0,…,5} such
 *            that every 2×2 subblock has the sum of the squares of the six
 *            edge and diagonal differences equal to 11. The value is computed
 *            by dynamic programming over all 216 possible row states (triples
 *            0..5)
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_STATES 216   /* 6³ possible rows */

/* Check whether a single 2×2 block satisfies the condition. */
static inline bool valid_block(int a, int b, int c, int d)
{
    int d0 = a - b; d0 *= d0;          /* horizontal top   */
    int d1 = c - d; d1 *= d1;          /* horizontal bottom */
    int d2 = a - c; d2 *= d2;          /* vertical left    */
    int d3 = b - d; d3 *= d3;          /* vertical right   */
    int d4 = a - d; d4 *= d4;          /* diagonal down‑right  */
    int d5 = b - c; d5 *= d5;          /* diagonal down‑left   */
    return (d0 + d1 + d2 + d3 + d4 + d5) == 11;
}

/* ------------------------------------------------------------------------
   Sequence function
   ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------
       Build the adjacency lists the first time the function is called.
       -------------------------------------------------------------- */
    static bool initialized = false;
    static unsigned short adj_start[MAX_STATES + 1];
    static unsigned short adj_cnt[MAX_STATES];
    static unsigned short adj_flat[MAX_STATES * MAX_STATES];

    if (!initialized) {
        unsigned short pos = 0;
        for (unsigned short sa = 0; sa < MAX_STATES; ++sa) {
            unsigned int sa0 = sa / 36;          /* aₙᵢ */
            unsigned int rem_a = sa % 36;
            unsigned int sa1 = rem_a / 6;        /* bₙᵢ */
            unsigned int sa2 = rem_a % 6;        /* cₙᵢ */

            unsigned short start = pos;
            for (unsigned short sb = 0; sb < MAX_STATES; ++sb) {
                unsigned int sb0 = sb / 36;      /* dₙₖ */
                unsigned int rem_b = sb % 36;
                unsigned int sb1 = rem_b / 6;
                unsigned int sb2 = rem_b % 6;

                if (valid_block((int)sa0, (int)sa1, (int)sb0, (int)sb1) &&
                    valid_block((int)sa1, (int)sa2, (int)sb1, (int)sb2)) {
                    adj_flat[pos++] = sb;
                }
            }
            adj_start[sa] = start;
            adj_cnt[sa]   = pos - start;
        }
        adj_start[MAX_STATES] = pos;   /* sentinel for possible future use */
        initialized = true;
    }

    /* --------------------------------------------------------------
       Dynamic programming over rows.
       -------------------------------------------------------------- */
    static seq_t dp_arr[MAX_STATES];
    static seq_t dp_next_arr[MAX_STATES];
    seq_t *dp      = dp_arr;
    seq_t *dp_next = dp_next_arr;

    /* Initial row: any state is possible */
    for (unsigned short i = 0; i < MAX_STATES; ++i)
        dp[i] = (seq_t)1;

    /* Perform n steps (each step adds one more row) */
    for (idx_t step = 0; step < n; ++step) {
        /* clear dp_next */
        for (unsigned short i = 0; i < MAX_STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate counts */
        for (unsigned short s = 0; s < MAX_STATES; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            unsigned short base  = adj_start[s];
            unsigned short count = adj_cnt[s];
            for (unsigned short k = 0; k < count; ++k) {
                unsigned short t = adj_flat[base + k];
                dp_next[t] += val;
            }
        }

        /* swap pointers for next iteration */
        seq_t *temp = dp;
        dp = dp_next;
        dp_next = temp;
    }

    /* Sum all final state counts */
    seq_t total = (seq_t)0;
    for (unsigned short i = 0; i < MAX_STATES; ++i)
        total += dp[i];

    return total;
}
