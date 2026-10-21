/*
 * A229006  Number of arrays of median of three adjacent elements of some
 *          length n+2 0..2 array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229006: number of arrays of medians of three adjacent
 * elements of an array of length n+2 over {0,1,2} with no equal
 * adjacent elements.  The values are obtained by determinising the
 * automaton that models the median operation; the DFA has 64 states
 * (subsets of the 6 possible adjacent‑symbol pairs).  The function
 * counts the number of distinct label strings of length n accepted
 * by this DFA.
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* the offset of the sequence is 1; a(0) is irrelevant */
    if (n == 0) return 0;

    /* Persistent tables for the DFA construction */
    static int  state_a[6], state_b[6], idx_of[3][3];
    static unsigned char dest_mask[6][3];
    static unsigned char delta[64][3];
    static int initialized = 0;

    if (!initialized)
    {
        /* initialise mapping from state pair (a,b) to index */
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                idx_of[i][j] = -1;

        state_a[0] = 0; state_b[0] = 1;
        state_a[1] = 0; state_b[1] = 2;
        state_a[2] = 1; state_b[2] = 0;
        state_a[3] = 1; state_b[3] = 2;
        state_a[4] = 2; state_b[4] = 0;
        state_a[5] = 2; state_b[5] = 1;
        for (int i = 0; i < 6; ++i)
            idx_of[state_a[i]][state_b[i]] = i;

        /* pre‑compute destination sets for each NFA state and median letter */
        for (int i = 0; i < 6; ++i)
        {
            int a = state_a[i], b = state_b[i];
            dest_mask[i][0] = dest_mask[i][1] = dest_mask[i][2] = 0;
            for (int c = 0; c < 3; ++c)
            {
                if (c == b) continue;                /* no equal adjacent */
                int x = a, y = b, z = c;
                int median;
                /* compute the median of x,y,z */
                if ((x <= y && y <= z) || (z <= y && y <= x))
                    median = y;
                else if ((y <= x && x <= z) || (z <= x && x <= y))
                    median = x;
                else
                    median = z;
                int dest = idx_of[b][c];             /* new state (b,c) */
                dest_mask[i][median] |= 1u << dest;
            }
        }

        /* determinise the NFA: transition function on subsets of states */
        for (int mask = 0; mask < 64; ++mask)
        {
            for (int l = 0; l < 3; ++l)
            {
                unsigned char newmask = 0;
                for (int s = 0; s < 6; ++s)
                    if (mask & (1u << s))
                        newmask |= dest_mask[s][l];
                delta[mask][l] = newmask;
            }
        }
        initialized = 1;
    }

    /* DP over the DFA */
    static seq_t dp_cur[64];
    static seq_t dp_next[64];

    /* initialise current DP state */
    for (int i = 0; i < 64; ++i) dp_cur[i] = 0;
    const unsigned char start_mask = 0x3Fu;            /* all 6 NFA states */
    dp_cur[start_mask] = 1;

    for (idx_t step = 0; step < n; ++step)
    {
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        for (int s = 0; s < 64; ++s)
        {
            seq_t cur = dp_cur[s];
            if (cur == 0) continue;
            for (int l = 0; l < 3; ++l)
            {
                unsigned char ns = delta[s][l];
                if (ns == 0) continue;           /* no valid continuation */
                dp_next[ns] += cur;
            }
        }

        /* swap dp_cur and dp_next */
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all reachable DFA states */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        total += dp_cur[i];
    return total;
}
