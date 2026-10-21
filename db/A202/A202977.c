/*
 * A202977  Number of nX6 0..1 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A202977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   compute_pending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation: Count the number of n×6 binary arrays in which
 * every non‑zero element has at least two 1‑neighbors among its
 * horizontal (left/right) and vertical (up/down) neighbors.
 * The algorithm is a breadth‑first dynamic programming over rows.
 * See OEIS A202977 for definition and first terms.
 */

#include <stdbool.h>
#include "../utils/compute_pending.h"

#define MAX_STATES 4096           /* 2⁶×2⁶ (mask * 64 + pending) */
#define MAX_EDGES_PER_STATE 64    /* each mask may transition to all 64 masks */

/* Transition tables */
static unsigned short transition_to[MAX_STATES][MAX_EDGES_PER_STATE];
static unsigned char  transition_count[MAX_STATES];
static bool built = false;

/* Build all admissible transitions once. */
static void build_transitions(void)
{
    for (int src = 0; src < MAX_STATES; ++src)
    {
        int mask_prev     = src >> 6;           /* high 6 bits */
        int pending_prev  = src & 0x3F;         /* low 6 bits */
        int count = 0;

        for (int mask_cur = 0; mask_cur < 64; ++mask_cur)
        {
            /* 1. Check that all pending bits from previous row
               are satisfied by the new row. */
            if ((pending_prev & (~mask_cur)) != 0)
                continue;   /* a required 1 missing in new row */

            /* 2. Compute pending mask for the new row. */
            int pending_cur = compute_pending(mask_cur, mask_prev);
            if (pending_cur < 0)
                continue;   /* a 1 has 0 known neighbors -> invalid */

            /* 3. Accept transition. */
            int tgt = (mask_cur << 6) | pending_cur;
            transition_to[src][count++] = (unsigned short) tgt;
        }
        transition_count[src] = (unsigned char) count;
    }
    built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence defined for n >= 1 */
        return (seq_t)0;

    if (!built)
        build_transitions();

    static seq_t dp[MAX_STATES];
    static seq_t next_dp[MAX_STATES];

    seq_t *cur   = dp;
    seq_t *next  = next_dp;

    /* Initialise dp for the first row. */
    for (int i = 0; i < MAX_STATES; ++i)
        cur[i] = (seq_t)0;

    for (int mask0 = 0; mask0 < 64; ++mask0)
    {
        int pending0 = compute_pending(mask0, 0);  /* no up neighbours */
        if (pending0 >= 0)
        {
            int state = (mask0 << 6) | pending0;
            cur[state] = (seq_t)1;
        }
    }

    /* Process remaining rows. */
    for (idx_t row = 1; row < n; ++row)
    {
        /* zero next array */
        for (int i = 0; i < MAX_STATES; ++i)
            next[i] = (seq_t)0;

        for (int src = 0; src < MAX_STATES; ++src)
        {
            if (cur[src] == (seq_t)0)
                continue;
            unsigned char cnt = transition_count[src];
            for (unsigned char k = 0; k < cnt; ++k)
            {
                int tgt = transition_to[src][k];
                next[tgt] += cur[src];
            }
        }
        /* swap current and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over states where the last row has no pending ones. */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < 64; ++mask)
    {
        int state = (mask << 6);  /* pending = 0 */
        result += cur[state];
    }
    return result;
}
