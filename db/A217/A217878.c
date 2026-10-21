/*
 * A217878  Number of n-element 0..2 arrays with each element the minimum of 3
 *          adjacent elements of a random 0..2 array of n+2 elements.
 * https://oeis.org/A217878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 58, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n‑1) - 3*a(n‑2) + a(n‑3) + 3*a(n‑4) - a(n‑5) + a(n‑6) + a(n‑7)
 * with a(1)=3, a(2)=9, a(3)=22, a(4)=46, a(5)=91, a(6)=183, a(7)=383.
 *
 * This implementation builds the determinized DFA whose states are subsets of
 * the 9 possible consecutive pairs (b[i],b[i+1]) of a 0…2 array.  For each
 * input symbol a[i] the state deterministically transitions by merging all
 * triples (x,y,z) with min(x,y,z)=a[i].  The number of words of length n is
 * the total number of paths of length n in this DFA, i.e. the sum of the
 * counts in all non‑empty states after processing n symbols.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range indices.  The OEIS offset is 1. */
    if (n < 1) return 0;

    /* Number of possible pairs (b[i],b[i+1]) for 3 values = 3*3 = 9. */
    enum { NUM_PAIRS = 9, NUM_STATES = 1 << NUM_PAIRS };

    /* Transition table and helper masks are computed only once. */
    static int initialized = 0;
    static uint16_t state_to_mask[NUM_PAIRS][3];
    static uint16_t trans[NUM_STATES][3];
    static seq_t dp_cur[NUM_STATES];
    static seq_t dp_next[NUM_STATES];

    if (!initialized) {
        /* Build state_to_mask[ pair ][ min ] = bitmask of target pairs. */
        for (int p = 0; p < NUM_PAIRS; ++p)
            for (int m = 0; m < 3; ++m)
                state_to_mask[p][m] = 0;

        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b) {
                int s = a * 3 + b;            /* current pair (a,b) */
                for (int c = 0; c < 3; ++c) {
                    int m = a;                 /* minimum of a,b,c */
                    if (b < m) m = b;
                    if (c < m) m = c;
                    int t = b * 3 + c;         /* resulting pair (b,c) */
                    state_to_mask[s][m] |= (uint16_t)(1 << t);
                }
            }

        /* Build determinized transition table. */
        for (int mask = 0; mask < NUM_STATES; ++mask) {
            for (int sym = 0; sym < 3; ++sym) {
                uint16_t next = 0;
                for (int p = 0; p < NUM_PAIRS; ++p)
                    if (mask & (1 << p))
                        next |= state_to_mask[p][sym];
                trans[mask][sym] = next;
            }
        }
        initialized = 1;
    }

    /* DP buffers.  cur points to the current state counts. */
    seq_t *cur = dp_cur, *next = dp_next;

    /* Clear dp arrays and set the start state (all pairs). */
    for (int i = 0; i < NUM_STATES; ++i) {
        cur[i] = 0;
        next[i] = 0;
    }
    cur[NUM_STATES - 1] = 1;   /* mask with all 9 bits set */

    /* Run the DP for n steps. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next buffer. */
        for (int i = 0; i < NUM_STATES; ++i) next[i] = 0;

        /* Transition from each state for each input symbol. */
        for (int mask = 0; mask < NUM_STATES; ++mask) {
            seq_t val = cur[mask];
            if (!val) continue;
            for (int sym = 0; sym < 3; ++sym) {
                uint16_t dst = trans[mask][sym];
                next[dst] += val;
            }
        }
        /* Swap buffers. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum counts in all non‑empty states (the empty state indicates an
       impossible prefix and must not be counted). */
    seq_t ans = 0;
    for (int mask = 1; mask < NUM_STATES; ++mask)
        ans += cur[mask];

    return ans;
}
